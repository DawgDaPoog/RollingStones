// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "SoftTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API ASoftTile : public AStopTile
{
	GENERATED_BODY()


	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player);

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player);
	

};
