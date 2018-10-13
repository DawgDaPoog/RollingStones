
// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "StopTile.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "RollingStonesBall.h"
#include "Particles/ParticleSystemComponent.h"
#include "EnemyBall.h"
#include "Projectile.h"

// Sets default values
AStopTile::AStopTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetNotifyRigidBodyCollision(true);
	RootComponent = Mesh;

	Glow = CreateDefaultSubobject<UParticleSystemComponent>(FName("Glow"));
	Glow->SetupAttachment(RootComponent);

	OnDestructionParticleEffects = CreateDefaultSubobject<UParticleSystemComponent>(FName("Explosion On Destruction"));
	OnDestructionParticleEffects->SetupAttachment(RootComponent);
	OnDestructionParticleEffects->bAutoActivate = false;

	Tags.Add(FName("StopTile"));
}

// Called when the game starts or when spawned
void AStopTile::BeginPlay()
{
	Super::BeginPlay();
	
}




void AStopTile::ReactToPlayerOnHit(ARollingStonesBall* Player)
{
	Player->ResetMovement();
	Player->EnableMovementTimer();
}

void AStopTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	Destroy();
	ReactToPlayerOnHit(Player);
}

void AStopTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	EnemyBall->ResetMovement();
	EnemyBall->InitiateNextStepTimer();
	EnemyBall->EnableMoving();
}

void AStopTile::ReactToProjectile(AProjectile * Projectile)
{
	Projectile->StartDestroySequence();
}

void AStopTile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other->ActorHasTag(FName("Player")))
	{
		if (Cast<ARollingStonesBall>(Other)->bIsEmpowered)
		{
			ReactToEmpoweredPlayerOnHit(Cast<ARollingStonesBall>(Other));
		}
		else
		{
			ReactToPlayerOnHit(Cast<ARollingStonesBall>(Other));
		}
	}
	if (Other->ActorHasTag(FName("EnemyBall")))
	{
		ReactToEnemyBall(Cast<AEnemyBall>(Other));
	}
}

void AStopTile::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Projectile")))
	{
		ReactToProjectile(Cast<AProjectile>(OtherActor));
	}
}





