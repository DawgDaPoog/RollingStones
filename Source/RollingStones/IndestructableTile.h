// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "IndestructableTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AIndestructableTile : public AStopTile
{
	GENERATED_BODY()
	
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;


	
	
};
