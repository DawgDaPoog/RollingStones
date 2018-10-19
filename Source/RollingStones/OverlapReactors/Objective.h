// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "Objective.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AObjective : public AOverlapReactor
{
	GENERATED_BODY()
public:

	AObjective();

	UPROPERTY(EditInstanceOnly)
	class AExitTile* ExitTile;
protected:

	virtual void BeginPlay() override;

	virtual void ReactToPlayer(class ARollingStonesBall* Player) override;

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;
	
};
