// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "PlusEmpowerPuckup.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"

APlusEmpowerPuckup::APlusEmpowerPuckup()
{
	bIsBobbing = true;
}

void APlusEmpowerPuckup::ReactToPlayer(ARollingStonesBall * Player)
{
	Player->SetAmountOfEmpowersLeft(Player->GetAmountOfEmpowersLeft() + 1);
	Destroy();
}
