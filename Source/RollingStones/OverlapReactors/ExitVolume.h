// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "ExitVolume.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AExitVolume : public AOverlapReactor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRollingStonesBallMove();
	
	int AmountOfSecretsInLevel = 0;

	int AmountOfFoundSecrets = 0;

	int AmountOfMoves = 0;

	virtual void ReactToPlayer(class ARollingStonesBall* Player);

	UPROPERTY(EditInstanceOnly, Category = "Exit Tile")
	class AExitTile* ExitTile;
public:
	void IncrementAmountOfSecretsInLevel();

	void IncrementAmountOfFoundSecrets();

	UFUNCTION(BlueprintImplementableEvent)
	void StartEndLevelSequence();

	UFUNCTION(BlueprintPure)
	int GetAmountOfSecretsInLevel();

	UFUNCTION(BlueprintPure)
	int GetAmountOfSecretsFound();

	UFUNCTION(BlueprintPure)
	int GetAmountOfMovesDone();

	UFUNCTION(BlueprintPure)
	int GetAmountOfGoalsChecked();

	UFUNCTION(BlueprintPure)
	int GetAmountOfGoals();
};
