// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ShooterTile.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "Engine/StaticMesh.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"
#include "Materials/Material.h"
#include "Components/PointLightComponent.h"

AShooterTile::AShooterTile()
{
	BreakMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Break Material"));
}
void AShooterTile::BreakTile()
{
	bBroken = true;
	if (BreakMaterial)
	{
		Mesh->SetMaterial(0, BreakMaterial);
	}
	bIsProjectileShotFromTheDown = false;
	bIsProjectileShotFromTheLeft = false;
	bIsProjectileShotFromTheRight = false;
	bIsProjectileShotFromTheUp = false;

	PointLightDown->SetIntensity(0.f);
	PointLightLeft->SetIntensity(0.f);
	PointLightRight->SetIntensity(0.f);
	PointLightUp->SetIntensity(0.f);
}

void AShooterTile::ShootProjectiles()
{
	float SpawnDistance = 90.f;
	if (!bBroken)
	{
		if (bIsProjectileShotFromTheDown)
		{
			FTransform DownProjectileTransform = FTransform(FRotator(0.f, 180.f, 0.f), GetActorLocation() + FVector(-SpawnDistance, 0.f, 0.f), FVector(1.f));
			AProjectile* SpawnedProjectileDown = GetWorld()->SpawnActor<AProjectile>(Projectile, DownProjectileTransform, FActorSpawnParameters());
			if (SpawnedProjectileDown)
			{
				SpawnedProjectileDown->SetXYFactors(-ProjectileSpeed, 0.f);
			}
		}
		if (bIsProjectileShotFromTheUp)
		{
			FTransform UpProjectileTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(SpawnDistance, 0.f, 0.f), FVector(1.f));
			AProjectile* SpawnedProjectileUp = GetWorld()->SpawnActor<AProjectile>(Projectile, UpProjectileTransform, FActorSpawnParameters());
			if (SpawnedProjectileUp)
			{
				SpawnedProjectileUp->SetXYFactors(ProjectileSpeed, 0.f);
			}
		}
		if (bIsProjectileShotFromTheRight)
		{
			FTransform RightProjectileTransform = FTransform(FRotator(0.f, 90.f, 0.f), GetActorLocation() + FVector(0.f, SpawnDistance, 0.f), FVector(1.f));
			AProjectile* SpawnedProjectileRight = GetWorld()->SpawnActor<AProjectile>(Projectile, RightProjectileTransform, FActorSpawnParameters());
			if (SpawnedProjectileRight)
			{
				SpawnedProjectileRight->SetXYFactors(0.f, ProjectileSpeed);
			}
		}
		if (bIsProjectileShotFromTheLeft)
		{
			FTransform LeftProjectileTransform = FTransform(FRotator(0.f, -90.f, 0.f), GetActorLocation() + FVector(0.f, -SpawnDistance, 0.f), FVector(1.f));
			AProjectile* SpawnedProjectileLeft = GetWorld()->SpawnActor<AProjectile>(Projectile, LeftProjectileTransform, FActorSpawnParameters());
			if (SpawnedProjectileLeft)
			{
				SpawnedProjectileLeft->SetXYFactors(0.f, -ProjectileSpeed);
			}
		}
	}
}

void AShooterTile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ShootTimer, this, &AShooterTile::ShootProjectiles, ShootingRate, true, ShootingRate);
}

void AShooterTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	AStopTile::ReactToPlayerOnHit(Player);
}

void AShooterTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	BreakTile();
	AStopTile::ReactToPlayerOnHit(Player);
}

void AShooterTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	AStopTile::ReactToEnemyBall(EnemyBall);
}

void AShooterTile::ReactToProjectile(AProjectile * Projectile)
{
	BreakTile();
	Projectile->StartDestroySequence();
	UE_LOG(LogTemp, Warning, TEXT("Collided with projectile"));
}
