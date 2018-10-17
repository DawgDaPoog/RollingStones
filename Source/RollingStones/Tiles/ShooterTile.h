// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/ProjectileTile.h"
#include "ShooterTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AShooterTile : public AProjectileTile
{
	GENERATED_BODY()
public:
	AShooterTile();

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float ShootingRate = 2.f;

	void BreakTile();

	void ShootProjectiles();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Material")
	class UMaterial* BreakMaterial;

	
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;

	virtual void ReactToProjectile(class AProjectile* Projectile) override;
	
	bool bBroken = false;

	// Timer handle for shooting
	struct FTimerHandle ShootTimer;
	
};
