// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileDropMechanic.generated.h"

/**
* This is the class to implement the "Tile Droping" mechanic which drops the chosen tile onto the ground where the player wants.
*/
class AStopTile;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGSTONES_API UTileDropMechanic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileDropMechanic();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADropTileDud> DropTileDud;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initiate();

	void DropSelectedTile();

	bool IsInitiated() { return bInitiated; }

	void SetSelectedIndex(int32 IndexToSet);

	int32 GetSelectedIndex();
private:
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 2.f;

	bool bInitiated = false;
		
	int32 SelectedTileIndex = 0;
	
	bool bIsCoolingDown = false;

	struct FTimerHandle CooldownTimer;

	void StartCooldownTimer();
};
