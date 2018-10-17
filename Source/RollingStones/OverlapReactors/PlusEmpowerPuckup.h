// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "PlusEmpowerPuckup.generated.h"

/**
 *  An overlap reactor that gives +1 Empower use to a player upon picking up
 */
UCLASS()
class ROLLINGSTONES_API APlusEmpowerPuckup : public AOverlapReactor
{
	GENERATED_BODY()
		APlusEmpowerPuckup();

		virtual void ReactToPlayer(class ARollingStonesBall* Player) override;

	
	
};
