// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "AttractorBox.generated.h"

/**
 *   A box that forms a tile grid, making it so that the player is restrained to the tiles that he's supposed to be in
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
	
};
