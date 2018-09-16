// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "KillerTile.h"
#include "RollingStonesBall.h"

void AKillerTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	Player->Destroy();
}

void AKillerTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	Player->Destroy();
}
