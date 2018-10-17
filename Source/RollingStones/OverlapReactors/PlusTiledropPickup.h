// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "PlusTiledropPickup.generated.h"

/**
 *  An overlap reactor that gives +1 Tiledrop use to a player upon picking up
 */
UCLASS()
class ROLLINGSTONES_API APlusTiledropPickup : public AOverlapReactor
{
	GENERATED_BODY()
		APlusTiledropPickup();
	
		virtual void ReactToPlayer(class ARollingStonesBall* Player) override;
	
};
