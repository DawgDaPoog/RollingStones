// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/StopTile.h"
#include "SoftTile.generated.h"

/**
 * A soft tile. When a player or NPC hits this tile it is destroyed and a ball assumes it's place. In case of the enchanced movement the player just keeps on moving through the tile, destroying it.
 */
UCLASS()
class ROLLINGSTONES_API ASoftTile : public AStopTile
{
	GENERATED_BODY()


	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player);

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player);
	
	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;
};
