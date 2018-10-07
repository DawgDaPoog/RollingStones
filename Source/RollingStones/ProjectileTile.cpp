// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ProjectileTile.h"
#include "Projectile.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"

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
	// Shoot the projectiles (Spawn them with a velocity)
	Destroy();
	LaunchProjectiles();
	Super::ReactToPlayerOnHit(Player);
}

void AProjectileTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{

}

void AProjectileTile::LaunchProjectiles()
{
	float ProjectileSpeed = 400.f;
	if (bIsProjectileShotFromTheDown)
	{
		FTransform DownProjectileTransform = FTransform(FRotator(180.f, 0.f, 0.f), GetActorLocation() + FVector(-40.f, 0.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileDown = GetWorld()->SpawnActor<AProjectile>(Projectile, DownProjectileTransform, FActorSpawnParameters());
		SpawnedProjectileDown->SetXYFactors(-ProjectileSpeed, 0.f);
	}
	if (bIsProjectileShotFromTheUp)
	{
		FTransform UpProjectileTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(40.f, 0.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileUp = GetWorld()->SpawnActor<AProjectile>(Projectile, UpProjectileTransform, FActorSpawnParameters());
		SpawnedProjectileUp->SetXYFactors(ProjectileSpeed, 0.f);
	}
	if (bIsProjectileShotFromTheRight)
	{
		FTransform RightProjectileTransform = FTransform(FRotator(90.f, 0.f, 0.f), GetActorLocation() + FVector(0.f, 40.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileRight = GetWorld()->SpawnActor<AProjectile>(Projectile, RightProjectileTransform, FActorSpawnParameters());
		SpawnedProjectileRight->SetXYFactors(0.f, ProjectileSpeed);
	}
	if (bIsProjectileShotFromTheLeft)
	{
		FTransform LeftProjectileTransform = FTransform(FRotator(-90.f, 0.f, 0.f), GetActorLocation() + FVector(0.f, -40.f, 0.f), FVector(1.f));
		AProjectile* SpawnedProjectileLeft = GetWorld()->SpawnActor<AProjectile>(Projectile, LeftProjectileTransform, FActorSpawnParameters());
		SpawnedProjectileLeft->SetXYFactors(0.f, -ProjectileSpeed);
	}
}
