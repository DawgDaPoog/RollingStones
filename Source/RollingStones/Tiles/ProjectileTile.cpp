// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ProjectileTile.h"
#include "Projectile.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Components/StaticMeshComponent.h"
#include "../VolumesAndBounds/StopVolume.h"

AProjectileTile::AProjectileTile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PointLightDown = CreateDefaultSubobject<UPointLightComponent>(FName("PointLightDown"));
	PointLightDown->SetupAttachment(RootComponent);

	PointLightUp = CreateDefaultSubobject<UPointLightComponent>(FName("PointLightUp"));
	PointLightUp->SetupAttachment(RootComponent);

	PointLightLeft = CreateDefaultSubobject<UPointLightComponent>(FName("PointLightLeft"));
	PointLightLeft->SetupAttachment(RootComponent);

	PointLightRight = CreateDefaultSubobject<UPointLightComponent>(FName("PointLightRight"));
	PointLightRight->SetupAttachment(RootComponent);



}





void AProjectileTile::BeginPlay()
{
	Super::BeginPlay();

	PointLightDown->SetIntensity(0.f);
	PointLightUp->SetIntensity(0.f);
	PointLightLeft->SetIntensity(0.f);
	PointLightRight->SetIntensity(0.f);
}

void AProjectileTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PulseTheProjectileSpawnDirections(DeltaTime);


}

void AProjectileTile::PulseTheProjectileSpawnDirections(float DeltaTime)
{
	if (bIsProjectileShotFromTheDown)
	{
		ChangeTheIntensityOfActivePointLights(PointLightDown, DeltaTime);
	}

	if (bIsProjectileShotFromTheUp)
	{
		ChangeTheIntensityOfActivePointLights(PointLightUp, DeltaTime);
	}

	if (bIsProjectileShotFromTheLeft)
	{
		ChangeTheIntensityOfActivePointLights(PointLightLeft, DeltaTime);
	}
	if (bIsProjectileShotFromTheRight)
	{
		ChangeTheIntensityOfActivePointLights(PointLightRight, DeltaTime);
	}

	if (bLightIntensityGoesUp)
	{
		GlobalPointLightIntensity += IntensityChangeRate * DeltaTime;
	}
	else
	{
		GlobalPointLightIntensity -= IntensityChangeRate * DeltaTime;
	}


	if (GlobalPointLightIntensity <= 0.f) {
		bLightIntensityGoesUp = true;
	}
	if (GlobalPointLightIntensity >= 4000.f) {
		bLightIntensityGoesUp = false;
	}
}

void AProjectileTile::ChangeTheIntensityOfActivePointLights(UPointLightComponent* PointLight, float DeltaTime)
{
	if (bLightIntensityGoesUp)
	{
		PointLight->SetIntensity(GlobalPointLightIntensity + IntensityChangeRate * DeltaTime);
	}
	else
	{
		PointLight->SetIntensity(GlobalPointLightIntensity - IntensityChangeRate * DeltaTime);
	}

}

void AProjectileTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	Destroy();
	LaunchProjectiles();
	Super::ReactToPlayerOnHit(Player);
}

void AProjectileTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	// Destroy the tile and add the impulse towards the place where it has been
	Destroy();
	Player->bIsEmpowered = false;
	FVector PlayerToTileVector = GetActorLocation() - Player->GetActorLocation();
	PlayerToTileVector.Z = 0;
	Player->GetBall()->AddImpulse(PlayerToTileVector * 10000);

	// Check where the player is colliding with the tile and react appropriately
	if (StopVolume) {
		float boundry = 50.f;
		float UpperX = GetActorLocation().X + boundry;
		float LowerX = GetActorLocation().X - boundry;
		float UpperY = GetActorLocation().Y + boundry;
		float LowerY = GetActorLocation().Y - boundry;
		FVector PlayerLocation = Player->GetActorLocation();

		if (PlayerLocation.X < UpperX && PlayerLocation.X > LowerX && PlayerLocation.Y < LowerY) // Collided from the left side
		{
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 70, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
			bIsProjectileShotFromTheLeft = false;
		}
		else if (PlayerLocation.X > UpperX && PlayerLocation.Y > LowerY && PlayerLocation.Y < UpperY) // Collided from the down side
		{
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-70, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
			bIsProjectileShotFromTheUp = false;
		}
		else if (PlayerLocation.Y > UpperY && PlayerLocation.X > LowerX && PlayerLocation.X < UpperX) // Collided from the right side
		{
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -70, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
			bIsProjectileShotFromTheRight = false;
		}
		else if (PlayerLocation.X < LowerX && PlayerLocation.Y >LowerY && PlayerLocation.Y < UpperY) // Collided from the up side
		{
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(70, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
			bIsProjectileShotFromTheDown = false;
		}
	}

	LaunchProjectiles();
}

void AProjectileTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	Destroy();
	Super::ReactToEnemyBall(EnemyBall);
	LaunchProjectiles();
}

void AProjectileTile::ReactToProjectile(AProjectile * Projectile)
{
	Destroy();

	float boundry = 50.f;
	float UpperX = GetActorLocation().X + boundry;
	float LowerX = GetActorLocation().X - boundry;
	float UpperY = GetActorLocation().Y + boundry;
	float LowerY = GetActorLocation().Y - boundry;
	FVector ProjectileLocation = Projectile->GetActorLocation();

	if (ProjectileLocation.X < UpperX && ProjectileLocation.X > LowerX && ProjectileLocation.Y < LowerY) // Collided from the left side
	{
		bIsProjectileShotFromTheLeft = false;
	}
	else if (ProjectileLocation.X > UpperX && ProjectileLocation.Y > LowerY && ProjectileLocation.Y < UpperY) // Collided from the down side
	{
		bIsProjectileShotFromTheUp = false;
	}
	else if (ProjectileLocation.Y > UpperY && ProjectileLocation.X > LowerX && ProjectileLocation.X < UpperX) // Collided from the right side
	{
		bIsProjectileShotFromTheRight = false;
	}
	else if (ProjectileLocation.X < LowerX && ProjectileLocation.Y >LowerY && ProjectileLocation.Y < UpperY) // Collided from the up side
	{
		bIsProjectileShotFromTheDown = false;
	}

	Projectile->StartDestroySequence();
	LaunchProjectiles();
}

void AProjectileTile::LaunchProjectiles()
{
	float ProjectileSpeed = 400.f;
	if (bIsProjectileShotFromTheDown)
	{
		FTransform DownProjectileTransform = FTransform(FRotator(0.f, 180.f, 0.f), GetActorLocation() + FVector(-40.f, 0.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileDown = GetWorld()->SpawnActor<AProjectile>(Projectile, DownProjectileTransform, FActorSpawnParameters());
		if (SpawnedProjectileDown)
		{
			SpawnedProjectileDown->SetXYFactors(-ProjectileSpeed, 0.f);
		}
	}
	if (bIsProjectileShotFromTheUp)
	{
		FTransform UpProjectileTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(40.f, 0.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileUp = GetWorld()->SpawnActor<AProjectile>(Projectile, UpProjectileTransform, FActorSpawnParameters());
		if (SpawnedProjectileUp)
		{
			SpawnedProjectileUp->SetXYFactors(ProjectileSpeed, 0.f);
		}
	}
	if (bIsProjectileShotFromTheRight)
	{
		FTransform RightProjectileTransform = FTransform(FRotator(0.f, 90.f, 0.f), GetActorLocation() + FVector(0.f, 40.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileRight = GetWorld()->SpawnActor<AProjectile>(Projectile, RightProjectileTransform, FActorSpawnParameters());
		if (SpawnedProjectileRight)
		{
			SpawnedProjectileRight->SetXYFactors(0.f, ProjectileSpeed);
		}
	}
	if (bIsProjectileShotFromTheLeft)
	{
		FTransform LeftProjectileTransform = FTransform(FRotator(0.f, -90.f, 0.f), GetActorLocation() + FVector(0.f, -40.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileLeft = GetWorld()->SpawnActor<AProjectile>(Projectile, LeftProjectileTransform, FActorSpawnParameters());
		if (SpawnedProjectileLeft)
		{
			SpawnedProjectileLeft->SetXYFactors(0.f, -ProjectileSpeed);
		}
	}
}
