// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileDropMechanic.generated.h"

/**
* This is the class to implement the "Tile Droping" mechanic which drops the chosen tile onto the ground where the player wants.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGSTONES_API UTileDropMechanic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileDropMechanic();

	UPROPERTY(EditDefaultsOnly, Category = "TileDrops")
	TSubclassOf<class AStopTile> StopTile;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
