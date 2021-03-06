// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "StopVolume.generated.h"

/**
 * A stop volume that is to be utilised by tiles in order to achieve a proper effect of stopping the ball where it needs to be stopped on the grid.
 */
UCLASS()
class ROLLINGSTONES_API AStopVolume : public ATriggerBox
{
	GENERATED_BODY()

	AStopVolume();
	
	// When the volume is created, it's creator set's the name of the object that is supposed to be stopped by it. Is is done in such a way for other actors not to be accedentally stopped by this volume.
	FString ActorThatIsIntendedToBeStopped;
public:
	void SetIntendedActorToStop(FString ActorName);

	FString GetIntededActorToStop();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	
};
