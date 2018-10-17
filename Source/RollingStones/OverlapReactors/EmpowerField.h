// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "EmpowerField.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AEmpowerField : public AOverlapReactor
{
	GENERATED_BODY()
	
	virtual void ReactToPlayer(class ARollingStonesBall* Player);
	
	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall);
};
