// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "SaveGameRollingStones.h"

USaveGameRollingStones::USaveGameRollingStones()
{
	for (int i = 1; i <= 60; i++) {
		InHowManyStepsWasLevelFinished.Add(0);
		HowManySecretsWasFoundInALevel.Add(0);
	}
}


