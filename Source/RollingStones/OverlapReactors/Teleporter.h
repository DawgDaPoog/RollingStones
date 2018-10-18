// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "Teleporter.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API ATeleporter : public AOverlapReactor
{
	GENERATED_BODY()

	ATeleporter();
	
	UPROPERTY(EditInstanceOnly, Category = "Exit Location")
	AActor* ExitTeleporter;

protected:
	TArray<FString> ExitingActorNames;

	virtual void ReactToPlayer(class ARollingStonesBall* Player);

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall);
	
	virtual void NotifyActorEndOverlap(AActor * OtherActor) override;
};
