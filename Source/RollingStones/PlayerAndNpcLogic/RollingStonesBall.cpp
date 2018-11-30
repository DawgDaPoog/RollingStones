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

	// Create a spark trail that will follow the ball around
	SparkTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SparkTrail"));
	SparkTrail->bAutoActivate = false;
	SparkTrail->SetupAttachment(RootComponent);

	// Create an enchanced spark trail that is only active when empowered
	EnchancedSparkTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnchancedSparkTrail"));
	EnchancedSparkTrail->bAutoActivate = false;
	EnchancedSparkTrail->SetupAttachment(RootComponent);
	
	// Idle glow of the ball
	IdleGlow = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleGlow"));
	IdleGlow->bAutoActivate = true;
	IdleGlow->SetupAttachment(RootComponent);

	// Particle effects when charging up
	ChargeUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ChargeUpEffect"));
	ChargeUpEffect->bAutoActivate = false;
	ChargeUpEffect->SetupAttachment(RootComponent);
	ChargeUpEffect->bAbsoluteRotation = true;
	
	// Default subobject for a tiledrop mechanic
	TileDropMechanic = CreateDefaultSubobject<UTileDropMechanic>(FName("TileDropComponent"));

	// Tag for recognizing and reacting to the player
	Tags.Add(FName("Player"));
}

void ARollingStonesBall::BeginPlay()
{
	Super::BeginPlay();
	// Setting chargeup effect size to 0
	ChargeUpEffect->SetRelativeScale3D(FVector(0));

	// Creating a fluid camera to controll and placing it where it needs to be
	Camera = GetWorld()->SpawnActor<ACameraActor>(FVector(GetActorLocation() + FVector(-XCameraOffset, 0.f, ZCameraOffset)),FRotator(-80.f,0.f,0.f), FActorSpawnParameters());
	
	// Setting basic controller settings during play
	if (GetController()) {
		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;
		Cast<APlayerController>(GetController())->SetViewTarget(Camera);
	}


}

void ARollingStonesBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Adding force if we have movement active
	if (bMoving)
	{
		Ball->AddForce(FVector(UpMovement, RightMovement, 0));
	}

	// Finding overlapping actors and aligning ball to the grid
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

	// Upon reacting with the stoptile, play camera shake and stop moving
	if (MyShake && Other->ActorHasTag("StopTile"))
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 1.0f);
	
		//Finding the overlapping grid point and snaping our location to it
		GetOverlappingActors(OverlappingActors);
		for (auto Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("Pole"))
			{
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

void ARollingStonesBall::DecrementTileDropsLeft()
{
	AmountOfTiledropsLeft--;
}

void ARollingStonesBall::SmoothTheCamera(float DeltaTime)
{
	// Smooth camera movement to the point where it needs to be, based on ball's location
	float XDistance = Camera->GetActorLocation().X - (GetActorLocation().X - XCameraOffset);
	float YDistance = Camera->GetActorLocation().Y - GetActorLocation().Y;
	float ZDistance = Camera->GetActorLocation().Z - (GetActorLocation().Z + ZCameraOffset);

	// How fast the camera is moving towards it's destination
	const float VelocityAmplifier = 4.f;

	// Getting the velocities
	float XVelocity = -XDistance;
	float YVelocity = -YDistance;
	float ZVelocity = -ZDistance;

	// Setting camera's location slightly closer to it's destination based on the distance and velocity amplification
	Camera->SetActorLocation(FVector(Camera->GetActorLocation() + FVector(XVelocity, YVelocity, ZVelocity)*DeltaTime*VelocityAmplifier));
}

// Movement Section Start
void ARollingStonesBall::StartChargingMovement()
{
	// if the empowered movement is locked, or we have no more empowers left, return
	if (bIsEmpoweredMovementLocked || AmountOfEmpowersLeft <= 0) { return; }

	// Timer that is used to charge up our movement
	static FTimerHandle ChargeUpTimer;

	// if the ball is not moving and is not charging, start charging
	if (!bMoving && !bIsCharging)
	{
		bIsCharging = true;

		ChargeUpEffect->SetRelativeScale3D(FVector(0));

		GetWorldTimerManager().SetTimer(ChargeUpTimer, this, &ARollingStonesBall::CompleteChargeUp, 1.f, false, 1.5f);
		GetWorldTimerManager().SetTimer(ChargeUpParticleTimer, this, &ARollingStonesBall::IncreaseChargeUpParticleEffect, 0.08f, true, 0.f);
	}
	else if (!bMoving && bIsCharging) // Else, if we are not moving and are charging, stop charging
	{
		bIsCharging = false;

		GetWorldTimerManager().ClearTimer(ChargeUpTimer);
		GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);

		ChargeUpEffect->Deactivate();
	}
}


void ARollingStonesBall::StartMovement(bool IsMovingInX, bool IsNegative)
{
	// Stop charging, if we were
	bIsCharging = false;

	// Clear chargeup timer
	GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);

	// Deactivate chargeup particles
	ChargeUpEffect->Deactivate();

	// if the ball was charged, take of one point from our empowers, activate enchanced spark trail and empowered movement
	if (bIsCharged)
	{
		AmountOfEmpowersLeft--;
		EnchancedSparkTrail->Activate();
		bIsEmpowered = true;
	}

	// Setting values for our charged force, if we had it, and overall force
	float ChargedForce = bIsCharged * ForceApply / 2;
	float OverallForce = ChargedForce + ForceApply;

		// determine where this ball was told to move, based on the imputs, and set the movement variables. Add impulse.
	int32 sign = (IsNegative ? -1 : 1);
	RightMovement = sign * OverallForce * FGenericPlatformMath::Pow(1, (int)IsMovingInX);
	UpMovement = sign * OverallForce * (int)IsMovingInX;
	Ball->AddImpulse(FVector(sign * OverallForce * (int)IsMovingInX, sign * OverallForce * FGenericPlatformMath::Pow(1, (int)IsMovingInX), 0.f));

	// Set charged to false, in case we were
	bIsCharged = false;

	// We started moving
	bMoving = true;

	// Increase the particle speed of spark trail
	SparkTrail->CustomTimeDilation = 1;

	// Unlock the axis we will be moving on
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

	// Ignore the invisible walls, while we are moving
	Ball->SetCollisionProfileName(FName("IgnoreInvisibleWall"));
}

bool ARollingStonesBall::IsAStopTileBeside(FVector Direction)
{
	//
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

	// Cast a line trace to determine whether we have a tile beside us or not
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,        //result
		GetActorLocation(),    //start
		GetActorLocation()*Direction, //end
		ECC_Visibility, //collision channel
		RV_TraceParams
	);

	// If we hit something
	if (RV_Hit.bBlockingHit)
	{
		// If what we hit was a stop tile
		if (RV_Hit.GetActor()->ActorHasTag(FName("StopTile")))
		{
			// If the distance to it is less that 150 ( If we are less than one tile from it)
			if (FVector::Dist(GetActorLocation(), RV_Hit.GetActor()->GetActorLocation()) < 150) {
				// Then the tile is too close, return that we have a tile in that direction
				return true;
			}
			else {
				// If not, then the tile is far enough and we are able to move in that direction 
				return false;
			}

		}
	}
	else
	{
		// What we hit was not a tile, can move
		return false;
	}
	// Hit nothing, can move
	return false;
}

void ARollingStonesBall::ResetMovement()
{
	// Reseting movement variables, constraints and collisions to idle mode
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
	// Starting the movement timer
	static FTimerHandle EnableMovementTimer;
	GetWorldTimerManager().SetTimer(EnableMovementTimer, this, &ARollingStonesBall::EnableMovement, 0.2f, false);
}

// Functions reacting to the input, asking if we have a tile beside and if not moving, move
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
	// Enabling movement possibility
	bMoving = false;
}

void ARollingStonesBall::CompleteChargeUp()
{
	// Complete chargeup. Clear particle effect increase timer.
	if (bIsCharging) {
		bIsCharged = true;
		GetWorldTimerManager().ClearTimer(ChargeUpParticleTimer);
	}
}

void ARollingStonesBall::AlignToTheGrid()
{
	// Adding a force towards tile center, based on the distance to it
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
	// Making a player empowered, giving him additional movement values and an impulse, if we were not yet empowered
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
	// Redirecting the movement of ball backwards
	RightMovement = -RightMovement;
	UpMovement = -UpMovement;
	Ball->AddImpulse(FVector(UpMovement, RightMovement, 0.f));
}

void ARollingStonesBall::RedirectSideways(bool bRedirectRight)
{
	// Redirecting the movement of ball sideways
	if (bMovingInXZ)
	{
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
	// Sequence for dying when we are hit by something lethal
	Cast<APlayerController>(GetController())->SetInputMode(FInputModeUIOnly());

	// Deactivating all possible visual clues that we still might be in the world
	bMoving = false;
	ResetMovement();
	SparkTrail->Deactivate();
	IdleGlow->Deactivate();
	TileDropMechanic->DestroyComponent();
	Ball->SetVisibility(false);

	// Spawning the game over widget
	SpawnDeathScreenWidget();
}

 // Assisting Section End


// TileDrop Section Start
void ARollingStonesBall::InitiateTileDrop()
{
	// If tile dropping mechanic is locked or we have no tile drops left, return
	if (bIsTileDroppingLocked || AmountOfTiledropsLeft <=0) { return; }
	if (TileDropMechanic)
	{
		// Initiate tile drop attempt
		TileDropMechanic->Initiate();
	}
}

void ARollingStonesBall::TryTileDrop()
{
	// If tile dropping mechanic is locked or we have no tile drops left, return
	if (bIsTileDroppingLocked || AmountOfTiledropsLeft <= 0) { return; }
	if (TileDropMechanic)
	{
		// Tell a tile to drop selected tile
		TileDropMechanic->DropSelectedTile();
	}
}

// TileDrop Section End


// Remaining
void ARollingStonesBall::IncreaseChargeUpParticleEffect()
{
	// Increasing particle effect size
	ChargeUpEffect->Activate();
	ChargeUpEffect->SetRelativeScale3D(ChargeUpEffect->GetRelativeTransform().GetScale3D()+FVector(0.05));
}


void ARollingStonesBall::SetWholeViewCamera(ACameraActor * CameraToSet)
{
	// Setting the camera to blend to when we press whole view button
	WholeViewCamera = CameraToSet;
}

void SpawnDeathScreenWidget_Implementation()
{
}


