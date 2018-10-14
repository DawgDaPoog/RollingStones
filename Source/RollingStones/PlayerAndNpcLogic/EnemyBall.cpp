// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "EnemyBall.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

// Sets default values
AEnemyBall::AEnemyBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->BodyInstance.SetCollisionProfileName(FName("PhysicsActor"));
	Ball->SetSimulatePhysics(true);

	Ball->SetAngularDamping(1.f);
	Ball->SetLinearDamping(1.f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(FName("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	IdleGlow = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleGlow"));
	IdleGlow->bAutoActivate = true;
	IdleGlow->SetupAttachment(RootComponent);

	ExplosionOnDeath = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OnDeathParticles"));
	ExplosionOnDeath->bAutoActivate = false;
	ExplosionOnDeath->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyBall::BeginPlay()
{
	Super::BeginPlay();
	if (IdleGlow)
	{
		IdleGlow->SetRelativeLocation(FVector(0.f));
	}
	if (ExplosionOnDeath)
	{
		ExplosionOnDeath->SetRelativeLocation(FVector(0.f));
	}
	GetWorldTimerManager().SetTimer(NextMoveTimer, this, &AEnemyBall::InitiateNextStepMovement, 3.f, false);
}

// Called every frame
void AEnemyBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving)
	{
		Ball->AddForce(FVector(UpMovement, RightMovement, 0));
	}

	GetOverlappingActors(OverlappingActors);

	AlignToTheGrid();

	PositionTheArrow();

	RotateTheArrow();
}

void AEnemyBall::Die()
{
	Destroy();
}

void AEnemyBall::ResetMovement()
{
	UpMovement = 0;
	RightMovement = 0;
	bMovingInXZ = false;
	bMovingInYZ = false;
	Ball->SetConstraintMode(EDOFMode::None);
	Ball->SetCollisionProfileName(FName("PhysicsActor"));
}

void AEnemyBall::EnableMoving()
{
	bMoving = false;
	Arrow->SetVisibility(true);
}

void AEnemyBall::AlignToTheGrid()
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

void AEnemyBall::InitiateNextStepTimer()
{
	IterateTheOrder();
	GetWorldTimerManager().SetTimer(NextMoveTimer, this, &AEnemyBall::InitiateNextStepMovement, 3.f, false);
}

void AEnemyBall::RedirectBackwards()
{
	UpMovement = -UpMovement;
	RightMovement = -RightMovement;
	Ball->AddImpulse(FVector(UpMovement, RightMovement, 0.f));
}

void AEnemyBall::RedirectSideways(bool bRedirectRight)
{
	if (bMovingInXZ)
	{
		Ball->SetConstraintMode(EDOFMode::YZPlane);
		bMovingInXZ = false;
		bMovingInYZ = true;
		if (bRedirectRight)
		{
			Swap(UpMovement, RightMovement);
			Ball->AddImpulse(FVector(0.f, RightMovement, 0.f));
		}
		else
		{
			Swap(UpMovement, RightMovement);
			RightMovement = -RightMovement;
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
			Swap(UpMovement, RightMovement);
			UpMovement = -UpMovement;
			Ball->AddImpulse(FVector(UpMovement, 0.f, 0.f));
		}
		else
		{
			Swap(UpMovement, RightMovement);
			Ball->AddImpulse(FVector(UpMovement, 0.f, 0.f));
		}
	}
}

void AEnemyBall::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{

}

void AEnemyBall::NotifyActorBeginOverlap(AActor * OtherActor)
{
	/*if (OtherActor->ActorHasTag(FName("StopVolume"))) {
		bMoving = false;
		OtherActor->Destroy();
		ResetMovement();
	}*/
}

void AEnemyBall::InitiateNextStepMovement()
{
	if (MovementOrders.Num() != 0)
	{
		//Check for the movement order and if there is a tile beside the intended move order and if all checks out, move
		if (MovementOrders[MovementIterator] == EMovementOrder::Right && !IsAStopTileBeside(FVector(1.f, 10.f, 1.f)))
		{
			StartMovement(false, false);
			Arrow->SetVisibility(false);
		}
		else if (MovementOrders[MovementIterator] == EMovementOrder::Down && !IsAStopTileBeside(FVector(10.f, 1.f, 1.f)))
		{
			StartMovement(true, true);
			Arrow->SetVisibility(false);
		}
		else if (MovementOrders[MovementIterator] == EMovementOrder::Left && !IsAStopTileBeside(FVector(1.f, -10.f, 1.f)))
		{
			StartMovement(false, true);
			Arrow->SetVisibility(false);
		}
		else if (MovementOrders[MovementIterator] == EMovementOrder::Up && !IsAStopTileBeside(FVector(-10.f, 1.f, 1.f)))
		{
			StartMovement(true, false);
			Arrow->SetVisibility(false);
		}
		//If the above didn't go through, then there must be an obsticle in the intended direction
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Reinitialising Timer"));
			//Choose the next iteration
			InitiateNextStepTimer();
		}


		//if the above exited here, then all is fine and dandy and we started moving
	}
}

void AEnemyBall::IterateTheOrder()
{
	if (MovementOrders.Num() != 0)
	{
		if (MovementOrders.Num() == MovementIterator + 1) // we reached the last order 
		{
			MovementIterator = 0;
		}
		else MovementIterator++;
	}
	RotateTheArrow();
}

void AEnemyBall::StartMovement(bool IsMovingInX, bool IsNegative)
{
	if (IsNegative)
	{
		RightMovement = -ForceApply * FGenericPlatformMath::Pow(1, (int)IsMovingInX);
		UpMovement = -ForceApply * (int)IsMovingInX;
		Ball->AddImpulse(FVector(-ForceApply * (int)IsMovingInX, -ForceApply * FGenericPlatformMath::Pow(1, (int)IsMovingInX), 0.f));
	}
	else
	{
		RightMovement = ForceApply * FGenericPlatformMath::Pow(1, (int)IsMovingInX);
		UpMovement = ForceApply * (int)IsMovingInX;
		Ball->AddImpulse(FVector(ForceApply * (int)IsMovingInX, ForceApply * FGenericPlatformMath::Pow(1, (int)IsMovingInX), 0.f));
	}
	
	bMoving = true;

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

void AEnemyBall::RotateTheArrow()
{
	if (MovementOrders.Num() != 0)
	{
		if (Arrow)
		{
			if (MovementOrders[MovementIterator] == EMovementOrder::Down)
			{
				Arrow->SetWorldRotation(FRotator(-90.f, 0.f, 180.f));
			}
			if (MovementOrders[MovementIterator] == EMovementOrder::Up)
			{
				Arrow->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
			}
			if (MovementOrders[MovementIterator] == EMovementOrder::Right)
			{
				Arrow->SetWorldRotation(FRotator(-90.f, 0.f, 90.f));
			}
			if (MovementOrders[MovementIterator] == EMovementOrder::Left)
			{
				Arrow->SetWorldRotation(FRotator(-90.f, 0.f, -90.f));
			}
		}
	}
}

void AEnemyBall::PositionTheArrow()
{
	Arrow->SetWorldLocation(GetActorLocation() + FVector(0.f, 0.f, 70.f));
}

bool AEnemyBall::IsAStopTileBeside(FVector Direction)
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel
	(
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
			if (FVector::Dist(GetActorLocation(), RV_Hit.GetActor()->GetActorLocation()) < 150)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Tile is too close. Can't move that way"));
				return true;
			}
			else 
			{
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
