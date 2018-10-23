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
#include "../VolumesAndBounds/AttractorBox.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystemComponent.h"
#include "TileDropMechanic.h"
#include "Camera/CameraActor.h"


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

	//// Create a camera and attach to boom
	//Camera = CreateDefaultSubobject<ACameraActor>(TEXT("Camera0"));


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

void ARollingStonesBall::BeginPlay()
{
	Super::BeginPlay();
	ChargeUpEffect->SetRelativeScale3D(FVector(0));

	Camera = GetWorld()->SpawnActor<ACameraActor>(FVector(GetActorLocation() + FVector(-XCameraOffset, 0.f, ZCameraOffset)),FRotator(-80.f,0.f,0.f), FActorSpawnParameters());
	if (GetController()) {
		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;
		Cast<APlayerController>(GetController())->SetViewTarget(Camera);
	}


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

void ARollingStonesBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings

	// Movement Key Bindings
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARollingStonesBall::MoveRight);
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &ARollingStonesBall::MoveForward);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARollingStonesBall::MoveLeft);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARollingStonesBall::MoveDown);
	PlayerInputComponent->BindAction("StartOrCancelChargeUp", IE_Pressed, this, &ARollingStonesBall::StartChargingMovement);

	// Tiledrop Key Bindings
	PlayerInputComponent->BindAction("InitiateTileDrop", IE_Pressed, this, &ARollingStonesBall::InitiateTileDrop);
	PlayerInputComponent->BindAction("TryTileDrop", IE_Released, this, &ARollingStonesBall::TryTileDrop);
	PlayerInputComponent->BindAction("ChooseFirstTileDrop", IE_Pressed, this, &ARollingStonesBall::SetTiledropIndex<0>);
	PlayerInputComponent->BindAction("ChooseSecondTileDrop", IE_Pressed, this, &ARollingStonesBall::SetTiledropIndex<1>);
	PlayerInputComponent->BindAction("ChooseThirdTileDrop", IE_Pressed, this, &ARollingStonesBall::SetTiledropIndex<2>);


}

void ARollingStonesBall::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (MyShake && Other->ActorHasTag("StopTile"))
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 1.0f);
		UE_LOG(LogTemp, Warning, TEXT("I Hit %s"), *Other->GetName());
		GetOverlappingActors(OverlappingActors);
		for (auto Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("Pole"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Alligning"));
				auto GridLoc = Actor->GetActorLocation();
				FVector NewLoc = FVector(GridLoc.X, GridLoc.Y, GetActorLocation().Z);
				SetActorLocation(NewLoc);
			}
		}
	}

}

void ARollingStonesBall::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}



void ARollingStonesBall::SmoothTheCamera(float DeltaTime)
{
	float XDistance = Camera->GetActorLocation().X - (GetActorLocation().X - XCameraOffset);
	float YDistance = Camera->GetActorLocation().Y - GetActorLocation().Y;
	float ZDistance = Camera->GetActorLocation().Z - (GetActorLocation().Z + ZCameraOffset);

	float VelocityAmplifier = 4.f;

	float XVelocity = -XDistance;
	float YVelocity = -YDistance;
	float ZVelocity = -ZDistance;

	Camera->SetActorLocation(FVector(Camera->GetActorLocation() + FVector(XVelocity, YVelocity, ZVelocity)*DeltaTime*VelocityAmplifier));
}

// Movement Section Start
void ARollingStonesBall::StartChargingMovement()
{
	if (bIsEmpoweredMovementLocked) { return; }
	static FTimerHandle ChargeUpTimer;
	if (AmountOfEmpowersLeft <= 0) { return; }
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
		AmountOfEmpowersLeft--;
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
		bMovingInXZ = true;
	}
	else
	{
		Ball->SetConstraintMode(EDOFMode::YZPlane);
		bMovingInYZ = true;
	}
	Ball->SetCollisionProfileName(FName("IgnoreInvisibleWall"));
}

bool ARollingStonesBall::IsAStopTileBeside(FVector Direction)
{
	if (abs(Direction.X) > 2.f && GetActorLocation().X>0)
	{
		Direction.X = -Direction.X;
	}
	if (abs(Direction.Y) > 2.f && GetActorLocation().Y<0)
	{
		Direction.Y = -Direction.Y;
	}
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,        //result
		GetActorLocation(),    //start
		GetActorLocation()*Direction, //end
		ECC_Visibility, //collision channel
		RV_TraceParams
	);

	if (RV_Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Something: %s"), *RV_Hit.GetActor()->GetName());

		if (RV_Hit.GetActor()->ActorHasTag(FName("StopTile")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit a stop tile"));
			if (FVector::Dist(GetActorLocation(), RV_Hit.GetActor()->GetActorLocation()) < 150) {
				UE_LOG(LogTemp, Warning, TEXT("Tile is too close. Can't move that way"));
				return true;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Tile is far away. Can move that way"));
				return false;
			}

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Nothing"));
		return false;
	}
	return false;
}

void ARollingStonesBall::ResetMovement()
{
	UpMovement = 0;
	RightMovement = 0;
	bIsEmpowered = false;
	bMovingInXZ = false;
	bMovingInYZ = false;
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

void ARollingStonesBall::MoveRight()
{
	if (!bMoving && !IsAStopTileBeside(FVector(1.f, 5.f, 1.f))) {
		OnMove.Broadcast();
		StartMovement(false, false);
	}
}

void ARollingStonesBall::MoveForward()
{
	if (!bMoving && !IsAStopTileBeside(FVector(-5.f, 1.f, 1.f))) {
		OnMove.Broadcast();
		StartMovement(true, false);
	}
}

void ARollingStonesBall::MoveDown()
{
	if (!bMoving && !IsAStopTileBeside(FVector(5.f, 1.f, 1.f))) {
		OnMove.Broadcast();
		StartMovement(true, true);
	}
}


void ARollingStonesBall::MoveLeft()
{
	if (!bMoving && !IsAStopTileBeside(FVector(1.f, -5.f, 1.f))) {
		OnMove.Broadcast();
		StartMovement(false, true);
	}
}


bool ARollingStonesBall::IsEmpoweredMovementLocked()
{
	return bIsEmpoweredMovementLocked;
}

bool ARollingStonesBall::IsTileDroppingLocked()
{
	return bIsTileDroppingLocked;
}

void ARollingStonesBall::SetEmpoweredMovementLocked(bool Set)
{
	bIsEmpoweredMovementLocked = Set;
}

void ARollingStonesBall::SetTileDroppingLocked(bool Set)
{
	bIsTileDroppingLocked = Set;
}

void ARollingStonesBall::EnableMovement()
{
	bMoving = false;
}

void ARollingStonesBall::CompleteChargeUp()
{
	if (bIsCharging) {
		bIsCharged = true;
		GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);
	}
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
// Movement Section End



// Assisting Section Start (This section is mostly called by the classes from the outside to determine and cause an effect on the player)

bool ARollingStonesBall::IsMovingInXZ()
{
	return bMovingInXZ;
}

bool ARollingStonesBall::IsMovingInYZ()
{
	return bMovingInYZ;
}

void ARollingStonesBall::Empower()
{
	if (!bIsEmpowered)
	{
		if (bMovingInXZ)
		{
			bool bIsNegative = UpMovement < 0.f;
			UpMovement += pow(-1,bIsNegative)*ForceApply / 2;
		}
		if (bMovingInYZ)
		{
			bool bIsNegative = RightMovement < 0.f;
			RightMovement += pow(-1, bIsNegative)*ForceApply / 2;
		}
		Ball->AddImpulse(FVector(UpMovement/2, RightMovement/2, 0.f));
		bIsEmpowered = true;
		EnchancedSparkTrail->Activate();
	}
}

int32 ARollingStonesBall::GetAmountOfEmpowersLeft()
{
	return AmountOfEmpowersLeft;
}

void ARollingStonesBall::SetAmountOfEmpowersLeft(int32 ValueToSet)
{
	AmountOfEmpowersLeft = ValueToSet;
}

int32 ARollingStonesBall::GetAmountOfTiledropsLeft()
{
	return AmountOfTiledropsLeft;
}

void ARollingStonesBall::SetAmountOfTiledropsLeft(int32 ValueToSet)
{
	AmountOfTiledropsLeft = ValueToSet;
}

ACameraActor * ARollingStonesBall::GetWholeViewCamera()
{
	return WholeViewCamera;
}

void ARollingStonesBall::RedirectBackwards()
{
	RightMovement = -RightMovement;
	UpMovement = -UpMovement;
	Ball->AddImpulse(FVector(UpMovement, RightMovement, 0.f));
}

void ARollingStonesBall::RedirectSideways(bool bRedirectRight)
{
	if (bMovingInXZ)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovingInYZ now"));
		Ball->SetConstraintMode(EDOFMode::YZPlane);
		bMovingInXZ = false;
		bMovingInYZ = true;

		if (bRedirectRight)
		{
			RightMovement = UpMovement;
			UpMovement = 0.f;
			Ball->AddImpulse(FVector(0.f, RightMovement, 0.f));

		}
		else
		{
			RightMovement = -UpMovement;
			UpMovement = 0.f;
			Ball->AddImpulse(FVector(0.f, RightMovement, 0.f));

		}
	}
	else if (bMovingInYZ)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovingInXZ now"));
		Ball->SetConstraintMode(EDOFMode::XZPlane);
		bMovingInXZ = true;
		bMovingInYZ = false;

		if (bRedirectRight)
		{
			UpMovement = -RightMovement;
			RightMovement = 0.f;
			Ball->AddImpulse(FVector(UpMovement, 0.f, 0.f));
		}
		else
		{
			UpMovement = RightMovement;
			RightMovement = 0.f;
			Ball->AddImpulse(FVector(UpMovement, 0.f, 0.f));
		}

	}
}

void ARollingStonesBall::Die()
{
	Cast<APlayerController>(GetController())->SetInputMode(FInputModeUIOnly());
	bMoving = false;
	ResetMovement();
	SparkTrail->Deactivate();
	IdleGlow->Deactivate();
	TileDropMechanic->DestroyComponent();

	Ball->SetVisibility(false);
	SpawnDeathScreenWidget();
}

 // Assisting Section End

// TileDrop Section Start
void ARollingStonesBall::InitiateTileDrop()
{
	if (bIsTileDroppingLocked) { return; }
	if (TileDropMechanic)
	{
		TileDropMechanic->Initiate();
	}
}

void ARollingStonesBall::TryTileDrop()
{
	if (bIsTileDroppingLocked) { return; }
	if (TileDropMechanic)
	{
		TileDropMechanic->DropSelectedTile();
	}
}

// TileDrop Section End


// Remaining
void ARollingStonesBall::IncreaseChargeUpParticleEffect()
{
	ChargeUpEffect->Activate();
	ChargeUpEffect->SetRelativeScale3D(ChargeUpEffect->GetRelativeTransform().GetScale3D()+FVector(0.05));
}


void ARollingStonesBall::SetWholeViewCamera(ACameraActor * CameraToSet)
{
	WholeViewCamera = CameraToSet;
}

void SpawnDeathScreenWidget_Implementation()
{
}


