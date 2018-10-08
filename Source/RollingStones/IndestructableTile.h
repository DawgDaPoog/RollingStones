// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "IndestructableTile.generated.h"

/**
 * Non-destructable tile. Will mostly serve as a boundries of level or a skeleton.
 */
UCLASS()
class ROLLINGSTONES_API AIndestructableTile : public AStopTile
{
	GENERATED_BODY()
	
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;
	
	
};
