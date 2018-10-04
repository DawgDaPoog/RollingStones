// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ProjectileTile.h"

void AProjectileTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	Destroy();
	// Shoot the projectiles (Spawn them with a velocity)
	Super::ReactToPlayerOnHit(Player);
}

void AProjectileTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{

}
