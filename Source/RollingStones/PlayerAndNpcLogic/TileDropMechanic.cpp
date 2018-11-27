// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "TileDropMechanic.h"
#include "GameFramework/Pawn.h"
#include "../Tiles/GroundTile.h"
#include "GameFramework/PlayerController.h"
#include "../Tiles/StopTile.h"
#include "../OverlapReactors/DropTileDud.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "RollingStonesBall.h"



// Sets default values for this component's properties
UTileDropMechanic::UTileDropMechanic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	if (bInitiated)
	{
		static FHitResult CursorHitResult;
		Cast<APawn>(GetOwner())->GetController()->CastToPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
		if (CursorHitResult.GetActor()) {
			if (CursorHitResult.GetActor()->ActorHasTag("GroundTile")) {

				Cast<AGroundTile>(CursorHitResult.GetActor())->bHighlighted = true;
			}
		}
	}
}

void UTileDropMechanic::Initiate()
{
	if (!bIsCoolingDown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Initiating"));
		if (bInitiated)
		{
			bInitiated = false;
		}
		else
		{
			bInitiated = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Coudln't Initiate"));
	}
}

void UTileDropMechanic::DropSelectedTile()
{
	if (bInitiated)
	{
		static FHitResult CursorHitResult;
		Cast<APawn>(GetOwner())->GetController()->CastToPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
		if (CursorHitResult.GetActor()) {
			if (CursorHitResult.GetActor()->ActorHasTag("GroundTile")) {

				if (!DropTileDud) return;
				UE_LOG(LogTemp, Warning, TEXT("Spawning Dud"));
				FTransform DudTransform = FTransform(FRotator(0.f), CursorHitResult.GetActor()->GetActorLocation() + FVector(0.f, 0.f, 1000.f), FVector(1.f));
				auto SpawnedDud = GetWorld()->SpawnActor<ADropTileDud>(DropTileDud, DudTransform, FActorSpawnParameters());
				SpawnedDud->SetIndex(SelectedTileIndex);
				Cast<ARollingStonesBall>(GetOwner())->DecrementTileDropsLeft();

				Initiate();
				bIsCoolingDown = true;
				GetWorld()->GetTimerManager().SetTimer(CooldownTimer,this, &UTileDropMechanic::StartCooldownTimer, CooldownTime, false);
			}
		}
	}
}

void UTileDropMechanic::SetSelectedIndex(int32 IndexToSet)
{
	SelectedTileIndex = IndexToSet;
}

int32 UTileDropMechanic::GetSelectedIndex()
{
	return SelectedTileIndex;
}

void UTileDropMechanic::StartCooldownTimer()
{
	bIsCoolingDown = false;
}

