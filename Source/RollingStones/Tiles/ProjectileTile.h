// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/StopTile.h"
#include "ProjectileTile.generated.h"

/**
 *  A tile that shoots projectiles out of one to four sides upon player or NPC collision
 */
UCLASS()
class ROLLINGSTONES_API AProjectileTile : public AStopTile
{
	GENERATED_BODY()
	
public:

	AProjectileTile();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	class UPointLightComponent* PointLightRight;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	class UPointLightComponent* PointLightLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	class UPointLightComponent* PointLightUp;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	class UPointLightComponent* PointLightDown;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bIsProjectileShotFromTheRight = false;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bIsProjectileShotFromTheLeft = false;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bIsProjectileShotFromTheUp = false;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bIsProjectileShotFromTheDown = false;

	void ChangeTheIntensityOfActivePointLights(class UPointLightComponent* PointLight, float DeltaTime);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void PulseTheProjectileSpawnDirections(float DeltaTime);

	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;
	
	virtual void ReactToEnemyBall(AEnemyBall* EnemyBall) override;

	virtual void ReactToProjectile(class AProjectile* Projectile) override;
	
private:
	void LaunchProjectiles();

	float GlobalPointLightIntensity = 0.f;
	
	bool bLightIntensityGoesUp = true;

protected:
	UPROPERTY(EditDefaultsOnly)
	float IntensityChangeRate = 5000.f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 400.f;
};
