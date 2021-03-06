// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/KillerTile.h"
#include "ExitTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AExitTile : public AKillerTile
{
	GENERATED_BODY()
	
public:
	void IncrementGoalAmount();

	void DecrementGoalAmount();

	virtual void Tick(float DeltaTime) override;

	int GetGoalAmount();

	int GetCheckedGoalAmount();
private:
	int32 GoalAmount = 0;
	
	int32 CheckedGoalAmount = 0;
};
