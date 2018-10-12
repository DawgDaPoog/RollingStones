// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "RollingStonesBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "AttractorBox.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystemComponent.h"
#include "TileDropMechanic.h"

#include "DrawDebugHelpers.h"

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
	
	TileDropMechanic = CreateDefaultSubobject<UTileDropMechanic>(FName("TileDropComponent"));
	Tags.Add(FName("Player"));
}

void ARollingStonesBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(bMoving)
	Ball->AddForce(FVector(UpMovement, RightMovement, 0));

	GetOverlappingActors(OverlappingActors);

	AlignToTheGrid();

	// Smoothing the camera movement

	SmoothTheCamera(DeltaTime);
	
}

void ARollingStonesBall::SmoothTheCamera(float DeltaTime)
{
	float XDistance = Camera->RelativeLocation.X - (GetActorLocation().X - 600.f);
	float YDistance = Camera->RelativeLocation.Y - GetActorLocation().Y;
	float ZDistance = Camera->RelativeLocation.Z - (GetActorLocation().Z + 500.f);

	float VelocityAmplifier = 2.f;

	float XVelocity = -XDistance;
	float YVelocity = -YDistance;
	float ZVelocity = -ZDistance;

	Camera->RelativeLocation = FVector(Camera->RelativeLocation + FVector(XVelocity, YVelocity, ZVelocity)*DeltaTime*VelocityAmplifier);
}

void ARollingStonesBall::BeginPlay()
{
	Super::BeginPlay();
	ChargeUpEffect->SetRelativeScale3D(FVector(0));
	
	if (GetController()) {
		GetController()->CastToPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetController()->CastToPlayerController()->bShowMouseCursor = true;
	}

	Camera->RelativeLocation = FVector(GetActorLocation() + FVector(-600.f, 0.f, 500.f));
}



void ARollingStonesBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARollingStonesBall::MoveRight);
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed,this, &ARollingStonesBall::MoveForward);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARollingStonesBall::MoveLeft);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARollingStonesBall::MoveDown);
	PlayerInputComponent->BindAction("StartOrCancelChargeUp", IE_Pressed, this, &ARollingStonesBall::StartChargingMovement);
}

void ARollingStonesBall::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if (MyShake && Other->ActorHasTag("StopTile"))
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 1.0f);
	}
}

void ARollingStonesBall::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void ARollingStonesBall::EnableMovement()
{
	bMoving = false;
}

bool ARollingStonesBall::IsAStopTileBeside(FVector Direction)
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,        //result
		GetActorLocation(),    //start
		GetActorLocation()*Direction, //end
		ECC_Visibility, //collision channel
		RV_TraceParams
	);

	if (RV_Hit.bBlockingHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Something: %s"), *RV_Hit.GetActor()->GetName());

		if (RV_Hit.GetActor()->ActorHasTag(FName("StopTile")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit a stop tile"));
			if (FVector::Dist(GetActorLocation(), RV_Hit.GetActor()->GetActorLocation()) < 150) {
				//UE_LOG(LogTemp, Warning, TEXT("Tile is too close. Can't move that way"));
				return true;
			}
			else {
				//UE_LOG(LogTemp, Warning, TEXT("Tile is far away. Can move that way"));
				return false;
			}
			
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Nothing"));
		return false;
	}
	return false;
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
	if (!bMoving && !IsAStopTileBeside(FVector(1.f, 10.f, 1.f))) {
		StartMovement(false,false);
	}
}

void ARollingStonesBall::MoveForward()
{
	if (!bMoving && !IsAStopTileBeside(FVector(-10.f, 1.f, 1.f))) {
		StartMovement(true, false);
	}
}

void ARollingStonesBall::MoveDown()
{
	if (!bMoving && !IsAStopTileBeside(FVector(10.f, 1.f, 1.f))) {
		StartMovement(true, true);
	}
}


void ARollingStonesBall::MoveLeft()
{
	if (!bMoving && !IsAStopTileBeside(FVector(1.f, -10.f, 1.f))) {
		StartMovement(false, true);
	}
}

void ARollingStonesBall::StartChargingMovement()
{
	static FTimerHandle ChargeUpTimer;
	if (!bMoving && !bIsCharging) 
	{
		bIsCharging = true;
		
		ChargeUpEffect->SetRelativeScale3D(FVector(0));

		GetWorldTimerManager().SetTimer(ChargeUpTimer, this, &ARollingStonesBall::CompleteChargeUp, 1.f, false, 1.5f);
		GetWorldTimerManager().SetTimer(ChargeUpParticleTimer, this, &ARollingStonesBall::IncreaseChargeUpParticleEffect, 0.08f, true, 0.f);
	}
	else if (!bMoving && bIsCharging)
	{
		bIsCharging = false;

		GetWorldTimerManager().ClearTimer(ChargeUpTimer);
		GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);

		
		ChargeUpEffect->Deactivate();
	}
}


void ARollingStonesBall::StartMovement(bool IsMovingInX, bool IsNegative)
{
	bIsCharging = false;

	GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);

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
		UpMovement = -OverallForce * (int)IsMovingInX;
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

void ARollingStonesBall::EnableMovementTimer()
{
	static FTimerHandle EnableMovementTimer;

	GetWorldTimerManager().SetTimer(EnableMovementTimer, this, &ARollingStonesBall::EnableMovement, 0.2f, false);
}

void ARollingStonesBall::Die()
{
	GetController()->CastToPlayerController()->SetInputMode(FInputModeUIOnly());
	bMoving = false;
	ResetMovement();
	SparkTrail->Deactivate();
	IdleGlow->Deactivate();
	TileDropMechanic->DestroyComponent();

	Ball->SetVisibility(false);
	SpawnDeathScreenWidget();
}

void SpawnDeathScreenWidget_Implementation()
{
	// your code here
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
