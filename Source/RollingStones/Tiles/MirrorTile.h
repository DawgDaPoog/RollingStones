// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/StopTile.h"
#include "MirrorTile.generated.h"

/**
 * Mirror tile. Redirects a player
 */
UCLASS()
class ROLLINGSTONES_API AMirrorTile : public AStopTile
{
	GENERATED_BODY()

protected:
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;

	virtual void ReactToProjectile(class AProjectile* Projectile) override;
	
	
};
