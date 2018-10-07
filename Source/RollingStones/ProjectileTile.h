// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "ProjectileTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AProjectileTile : public AStopTile
{
	GENERATED_BODY()
	
public:

	AProjectileTile();

	void ChangeTheIntensityOfActivePointLights(class UPointLightComponent* PointLight, float DeltaTime);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void PulseTheProjectileSpawnDirections(float DeltaTime);

	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player);

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player);
	
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
private:
	void LaunchProjectiles();

	float GlobalPointLightIntensity = 0.f;
	
	bool bLightIntensityGoesUp = true;

	UPROPERTY(EditDefaultsOnly)
	float IntensityChangeRate = 5000.f;

	
};
