// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "TileDropMechanic.h"
#include "GameFramework/Pawn.h"
#include "GroundTile.h"
#include "GameFramework/PlayerController.h"


//TODO finish this class


// Sets default values for this component's properties
UTileDropMechanic::UTileDropMechanic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTileDropMechanic::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTileDropMechanic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	static FHitResult CursorHitResult;
	Cast<APawn>(GetOwner())->GetController()->CastToPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	if (CursorHitResult.GetActor()) {
		if (CursorHitResult.GetActor()->ActorHasTag("GroundTile")) {
			//UE_LOG(LogTemp, Warning, TEXT("Hit an actor with a GroundTile tag"));

			Cast<AGroundTile>(CursorHitResult.GetActor())->bHighlighted = true;
		}
	}
}

