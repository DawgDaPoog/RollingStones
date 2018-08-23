// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttractorGrid.generated.h"

class AAttractorVolume;

UCLASS()
class ROLLINGSTONES_API AAttractorGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttractorGrid();

	UPROPERTY(EditDefaultsOnly, Category = "Grid Setup")
	int32 GridSize = 10;
	
	UPROPERTY(EditDefaultsOnly, Category = "Grid Setup")
	float GridGap = 100.f;


	UPROPERTY(EditDefaultsOnly, Category = "Grid Setup")
	TSubclassOf<AActor> AttractorVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
