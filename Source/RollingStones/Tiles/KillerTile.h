// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/StopTile.h"
#include "KillerTile.generated.h"

/**
 * Tile that kills player and NPCs
 */
UCLASS()
class ROLLINGSTONES_API AKillerTile : public AStopTile
{
	GENERATED_BODY()

	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;

	virtual void ReactToProjectile(class AProjectile* Projectile) override;
	
	bool bImGoingUp = true;

	FVector StartingLocation;

public:

	AKillerTile();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
};
