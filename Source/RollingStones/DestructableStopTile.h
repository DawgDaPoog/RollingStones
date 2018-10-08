// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "DestructableStopTile.generated.h"

/**
 * Hard Tile. When the player hits it with the regular movement, this tile is destroyed and player is stopped. If the player collides with this tile while empowered, the player assumes it's place
 */

class ARollingStonesBall;
UCLASS()
class ROLLINGSTONES_API ADestructableStopTile : public AStopTile
{
	GENERATED_BODY()
	
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;
	
	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;
	
};
