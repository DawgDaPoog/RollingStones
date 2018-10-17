// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "EmpowerField.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"


void AEmpowerField::ReactToPlayer(ARollingStonesBall * Player)
{
	Player->Empower();
}

void AEmpowerField::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	// TODO add empowering on EnemyBall
}
