// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RollingStonesGameMode.generated.h"

UCLASS(minimalapi)
class ARollingStonesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARollingStonesGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USaveGameRollingStones* GameSave;

	UFUNCTION(BlueprintPure)
	int32 GetGameSaveLastLevelFinished();

	UFUNCTION(BlueprintPure)
	int32 GetGameSaveAmountOfTurnsTakenFor(int32 Level);

	UFUNCTION(BlueprintCallable)
	void SaveLastLevelFinished(int32 LastLevelFinished);

	UFUNCTION(BlueprintCallable)
	void SaveAmountOfTurnsForLevel(int32 Level,int32 AmountOfTurns);
};



