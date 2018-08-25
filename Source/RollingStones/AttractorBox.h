// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "AttractorBox.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class ROLLINGSTONES_API AAttractorBox : public ATriggerBox
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WallUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WallDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WallLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WallRight;

public:
	AAttractorBox();

	void LockWalls();

	void UnlockWalls();
	
};
