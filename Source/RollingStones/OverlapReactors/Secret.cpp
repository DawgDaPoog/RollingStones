// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "Secret.h"
#include "ExitVolume.h"


ASecret::ASecret()
{

}

void ASecret::BeginPlay()
{
	Super::BeginPlay();
	if (ExitVolume)
	{
		ExitVolume->IncrementAmountOfSecretsInLevel();
	}
}

void ASecret::ReactToPlayer(ARollingStonesBall * Player)
{
	if (ExitVolume)
	{
		ExitVolume->IncrementAmountOfFoundSecrets();
	}
	Destroy();
}

void ASecret::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	if (ExitVolume)
	{
		ExitVolume->IncrementAmountOfFoundSecrets();
	}
	Destroy();
}
