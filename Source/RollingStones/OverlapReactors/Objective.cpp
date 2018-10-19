// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "Objective.h"
#include "../Tiles/ExitTile.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"

AObjective::AObjective()
{
	ExitTile = CreateDefaultSubobject<AExitTile>(FName("ExitTile"));
}

void AObjective::BeginPlay()
{
	Super::BeginPlay();
	if (ExitTile)
	{
		ExitTile->IncrementGoalAmount();
	}
}

void AObjective::ReactToPlayer(ARollingStonesBall * Player)
{
	ExitTile->DecrementGoalAmount();
	Destroy();
}

void AObjective::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	ExitTile->DecrementGoalAmount();
	Destroy();
}
