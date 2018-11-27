// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ExitTile.h"
#include "Components/StaticMeshComponent.h"

void AExitTile::IncrementGoalAmount()
{
	GoalAmount++;
}

void AExitTile::DecrementGoalAmount()
{
	CheckedGoalAmount++;
	if (GoalAmount <= CheckedGoalAmount)
	{
		Mesh->SetVisibility(false);
		Mesh->SetCollisionProfileName("OverlapAllDynamic");
	}
}

void AExitTile::Tick(float DeltaTime)
{
}

int AExitTile::GetGoalAmount()
{
	return GoalAmount;
}

int AExitTile::GetCheckedGoalAmount()
{
	return CheckedGoalAmount;
}
