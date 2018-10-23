// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "Secret.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API ASecret : public AOverlapReactor
{
	GENERATED_BODY()

		ASecret();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditInstanceOnly)
	class AExitVolume* ExitVolume;

	virtual void ReactToPlayer(class ARollingStonesBall* Player) override;

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;
};
