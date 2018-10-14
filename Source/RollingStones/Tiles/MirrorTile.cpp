// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "MirrorTile.h"
#include "Projectile.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"

void AMirrorTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	Player->RedirectBackwards();
}

void AMirrorTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	Player->RedirectBackwards();
}

void AMirrorTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	EnemyBall->RedirectBackwards();
}

void AMirrorTile::ReactToProjectile(AProjectile * Projectile)
{
	float XFactor;
	float YFactor;

	Projectile->GetXYFactors(XFactor, YFactor);
	XFactor = -XFactor;
	YFactor = -YFactor;
	Projectile->SetXYFactors(XFactor, YFactor);

	FTransform ProjectileTransform = Projectile->GetTransform();
	Projectile->SetActorRotation(FRotator(0.f,ProjectileTransform.GetRotation().Rotator().Yaw + 180.f,0.f));
}
