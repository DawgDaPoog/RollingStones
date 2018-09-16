// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "KillerTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AKillerTile : public AStopTile
{
	GENERATED_BODY()
	
	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;

	
	
};