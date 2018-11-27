// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ExitVolume.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Tiles/ExitTile.h"

void AExitVolume::BeginPlay()
{
	Super::BeginPlay();

	auto Player = Cast<ARollingStonesBall>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->OnMove.AddUniqueDynamic(this, &AExitVolume::OnRollingStonesBallMove);
}

void AExitVolume::OnRollingStonesBallMove()
{
	UE_LOG(LogTemp, Warning, TEXT("Ball moved! Aye!"));
	AmountOfMoves++;
}

void AExitVolume::ReactToPlayer(ARollingStonesBall * Player)
{
	StartEndLevelSequence();
}

void AExitVolume::IncrementAmountOfSecretsInLevel()
{
	AmountOfSecretsInLevel++;
}

void AExitVolume::IncrementAmountOfFoundSecrets()
{
	AmountOfFoundSecrets++;
}

int AExitVolume::GetAmountOfSecretsInLevel()
{
	return AmountOfSecretsInLevel;
}

int AExitVolume::GetAmountOfSecretsFound()
{
	return AmountOfFoundSecrets;
}

int AExitVolume::GetAmountOfMovesDone()
{
	return AmountOfMoves;
}

int AExitVolume::GetAmountOfGoalsChecked()
{
	if (ExitTile)
	{
		return ExitTile->GetCheckedGoalAmount();
	}
	else
	{
		return 0;
	}
}

int AExitVolume::GetAmountOfGoals()
{
	if (ExitTile)
	{
		return ExitTile->GetGoalAmount();
	}
	else
	{
		return 0;
	}
}
