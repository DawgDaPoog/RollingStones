// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameRollingStones.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API USaveGameRollingStones : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Levels)
	int LastLevelFinished = 0;
	
	UPROPERTY(VisibleAnywhere, Category = Levels)
	TArray<int> InHowManyStepsWasLevelFinished;

	UPROPERTY(VisibleAnywhere, Category = Levels)
	TArray<int> HowManySecretsWasFoundInALevel;

	USaveGameRollingStones();
};
