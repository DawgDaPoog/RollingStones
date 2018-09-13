// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "RollingStonesBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "AttractorBox.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystemComponent.h"

ARollingStonesBall::ARollingStonesBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);

	Ball->SetAngularDamping(1.f);
	Ball->SetLinearDamping(1.f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	
	RootComponent = Ball;
	Ball->SetConstraintMode(EDOFMode::XYPlane);

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	SparkTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SparkTrail"));
	SparkTrail->bAutoActivate = false;
	SparkTrail->SetupAttachment(RootComponent);

	EnchancedSparkTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnchancedSparkTrail"));
	EnchancedSparkTrail->bAutoActivate = false;
	EnchancedSparkTrail->SetupAttachment(RootComponent);
	
	IdleGlow = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleGlow"));
	IdleGlow->bAutoActivate = true;
	IdleGlow->SetupAttachment(RootComponent);

	
	
	ChargeUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ChargeUpEffect"));
	ChargeUpEffect->bAutoActivate = false;
	ChargeUpEffect->SetupAttachment(RootComponent);
	ChargeUpEffect->bAbsoluteRotation = true;
	
	
	Tags.Add(FName("Player"));
}

void ARollingStonesBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(bMoving)
	Ball->AddForce(FVector(UpMovement, RightMovement, 0));

	GetOverlappingActors(OverlappingActors);

	AlignToTheGrid();
}

void ARollingStonesBall::BeginPlay()
{
	Super::BeginPlay();
	ChargeUpEffect->SetRelativeScale3D(FVector(0));
}



void ARollingStonesBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &ARollingStonesBall::MoveRight);
	PlayerInputComponent->BindAction("MoveForward", IE_Released,this, &ARollingStonesBall::MoveForward);
	PlayerInputComponent->BindAction("MoveLeft", IE_Released, this, &ARollingStonesBall::MoveLeft);
	PlayerInputComponent->BindAction("MoveDown", IE_Released, this, &ARollingStonesBall::MoveDown);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARollingStonesBall::StartChargingMovement);
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &ARollingStonesBall::StartChargingMovement);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARollingStonesBall::StartChargingMovement);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARollingStonesBall::StartChargingMovement);
}

void ARollingStonesBall::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if (Other->ActorHasTag(FName("StopTile"))) {
		bool IsAKeyDown = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::A);
		bool IsSKeyDown = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::S);
		bool IsWKeyDown = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::W);
		bool IsDKeyDown = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::D);

		// if the key is down when you hit a wall, start charging (buffering movement)
		if (IsAKeyDown || IsSKeyDown || IsWKeyDown || IsDKeyDown) StartChargingMovement();
	}
}

void ARollingStonesBall::NotifyActorBeginOverlap(AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
	if (OtherActor->ActorHasTag(FName("StopVolume"))) {
		bMoving = false;
		OtherActor->Destroy();
		ResetMovement();
	}
}

void ARollingStonesBall::CompleteChargeUp()
{
	if (bIsCharging) {
		bIsCharged = true;
		GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);
	}
}

void ARollingStonesBall::IncreaseChargeUpParticleEffect()
{
	ChargeUpEffect->Activate();
	ChargeUpEffect->SetRelativeScale3D(ChargeUpEffect->GetRelativeTransform().GetScale3D()+FVector(0.05));
}

void ARollingStonesBall::MoveRight()
{
	if (!bMoving) {
		StartMovement(false,false);
	}
}

void ARollingStonesBall::MoveForward()
{
	if (!bMoving) {
		StartMovement(true, false);
	}
}

void ARollingStonesBall::MoveDown()
{
	if (!bMoving) {
		StartMovement(true, true);
	}
}

void ARollingStonesBall::StartChargingMovement()
{
	if (!bMoving) 
	{
		bIsCharging = true;
		static FTimerHandle ChargeUpTimer;
		
		GetWorldTimerManager().SetTimer(ChargeUpTimer, this, &ARollingStonesBall::CompleteChargeUp, 1.f, false, 1.5f);
		GetWorldTimerManager().SetTimer(ChargeUpParticleTimer, this, &ARollingStonesBall::IncreaseChargeUpParticleEffect, 0.08f, true, 0.f);
	}
}

void ARollingStonesBall::MoveLeft()
{
	if (!bMoving) {
		StartMovement(false, true);
	}
}

void ARollingStonesBall::StartMovement(bool IsMovingInX, bool IsNegative)
{
	bIsCharging = false;
	GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);
	ChargeUpEffect->SetRelativeScale3D(FVector(0));
	ChargeUpEffect->Deactivate();

	if (bIsCharged)
	{
		EnchancedSparkTrail->Activate();
		bIsEmpowered = true;
	}

	float ChargedForce = bIsCharged * ForceApply / 2;
	float OverallForce = ChargedForce + ForceApply;
	if (IsNegative)
	{
		RightMovement = -OverallForce * FGenericPlatformMath::Pow(1, (int)IsMovingInX);
		UpMovement = -OverallForce  * (int)IsMovingInX;
		Ball->AddImpulse(FVector(-OverallForce * (int)IsMovingInX, -OverallForce * FGenericPlatformMath::Pow(1, (int)IsMovingInX), 0.f));
	}
	else
	{
		RightMovement = OverallForce * FGenericPlatformMath::Pow(1, (int)IsMovingInX);
		UpMovement = OverallForce * (int)IsMovingInX;
		Ball->AddImpulse(FVector(OverallForce * (int)IsMovingInX, OverallForce * FGenericPlatformMath::Pow(1, (int)IsMovingInX), 0.f));
	}

	bIsCharged = false;
	bMoving = true;
	SparkTrail->CustomTimeDilation = 1;

	if (IsMovingInX)
	{
		Ball->SetConstraintMode(EDOFMode::XZPlane);
	}
	else
	{
		Ball->SetConstraintMode(EDOFMode::YZPlane);
	}
	Ball->SetCollisionProfileName(FName("IgnoreInvisibleWall"));
}

void ARollingStonesBall::ResetMovement()
{
	UpMovement = 0;
	RightMovement = 0;
	bIsEmpowered = false;
	Ball->SetConstraintMode(EDOFMode::None);
	Ball->SetCollisionProfileName(FName("PhysicsActor"));
	SparkTrail->CustomTimeDilation = 0.02;
	EnchancedSparkTrail->Deactivate();
}

void ARollingStonesBall::AlignToTheGrid()
{
	for (AActor* OverlappingActor : OverlappingActors) {
		if (OverlappingActor->ActorHasTag(FName("AttractorVolume")) && !bMoving)
		{
			FVector Direction = OverlappingActor->GetActorLocation() - GetActorLocation();
			float Distance = FVector::Dist(OverlappingActor->GetActorLocation(), GetActorLocation());
			Ball->AddForce(Direction*Distance * 500);
		}
	}
}
