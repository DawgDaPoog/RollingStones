// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ExitTile.h"


void AExitTile::IncrementGoalAmount()
{
	GoalAmount++;
}

void AExitTile::DecrementGoalAmount()
{
	GoalAmount--;
	if (GoalAmount <= 0)
	{
		Destroy();
	}
}
