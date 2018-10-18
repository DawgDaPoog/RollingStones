// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "PlusTiledropPickup.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"


APlusTiledropPickup::APlusTiledropPickup()
{
	bIsBobbing = true;
	bIsSpinning = true;
}

void APlusTiledropPickup::ReactToPlayer(ARollingStonesBall * Player)
{
	Player->SetAmountOfTiledropsLeft(Player->GetAmountOfTiledropsLeft() + 1);
	Destroy();
}
