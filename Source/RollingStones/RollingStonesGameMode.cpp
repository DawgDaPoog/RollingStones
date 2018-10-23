// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RollingStonesGameMode.h"
#include "PlayerAndNpcLogic/RollingStonesBall.h"
#include "Engine.h"
#include "SaveGameRollingStones.h"
#include "Kismet/GameplayStatics.h"

ARollingStonesGameMode::ARollingStonesGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARollingStonesBall::StaticClass();
}

void ARollingStonesGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameSave = Cast<USaveGameRollingStones>(UGameplayStatics::CreateSaveGameObject(USaveGameRollingStones::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("0"), 0))
	{
		GameSave = Cast<USaveGameRollingStones>(UGameplayStatics::LoadGameFromSlot(TEXT("0"), 0));
	}
	else
	{ 
		UGameplayStatics::SaveGameToSlot(GameSave, TEXT("0"), 0);
	}
}

int32 ARollingStonesGameMode::GetGameSaveLastLevelFinished()
{
	return GameSave->LastLevelFinished;
}

int32 ARollingStonesGameMode::GetGameSaveAmountOfTurnsTakenFor(int32 Level)
{
	return GameSave->InHowManyStepsWasLevelFinished[Level];
}

void ARollingStonesGameMode::SaveLastLevelFinished(int32 LastLevelFinished)
{
	GameSave->LastLevelFinished = LastLevelFinished;
	UGameplayStatics::SaveGameToSlot(GameSave, TEXT("0"), 0);
}

void ARollingStonesGameMode::SaveAmountOfTurnsForLevel(int32 Level, int32 AmountOfTurns)
{
	GameSave->InHowManyStepsWasLevelFinished[Level] = AmountOfTurns;
	UGameplayStatics::SaveGameToSlot(GameSave, TEXT("0"), 0);
}

