// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "ExitVolume.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void AExitVolume::BeginPlay()
{
	Super::BeginPlay();

	auto Player = Cast<ARollingStonesBall>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->OnMove.AddUniqueDynamic(this, &AExitVolume::OnRollingStonesBallMove);
}

void AExitVolume::OnRollingStonesBallMove()
{
	UE_LOG(LogTemp, Warning, TEXT("Ball moved! Aye!"));
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
