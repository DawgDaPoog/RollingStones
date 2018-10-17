// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "PlusTiledropPickup.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"


APlusTiledropPickup::APlusTiledropPickup()
{
	bIsBobbing = true;
}

void APlusTiledropPickup::ReactToPlayer(ARollingStonesBall * Player)
{
	Player->SetAmountOfTiledropsLeft(Player->GetAmountOfEmpowersLeft() + 1);
	Destroy();
}
