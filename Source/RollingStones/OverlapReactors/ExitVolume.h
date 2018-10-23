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

	virtual void ReactToPlayer(class ARollingStonesBall* Player);
public:
	void IncrementAmountOfSecretsInLevel();

	void IncrementAmountOfFoundSecrets();

	UFUNCTION(BlueprintImplementableEvent)
	void StartEndLevelSequence();
};
