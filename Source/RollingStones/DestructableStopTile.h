// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "DestructableStopTile.generated.h"

/**
 * Hard Tile. 
 */

class ARollingStonesBall;
UCLASS()
class ROLLINGSTONES_API ADestructableStopTile : public AStopTile
{
	GENERATED_BODY()
	
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;
	
	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;
	
};
