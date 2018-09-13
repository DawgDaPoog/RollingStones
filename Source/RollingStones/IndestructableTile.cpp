// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "IndestructableTile.h"
#include "RollingStonesBall.h"
#include "Engine/StaticMesh.h"
#include "StopVolume.h"

void AIndestructableTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	if (Player->bIsEmpowered) {
		FVector PlayerToTileVector = GetActorLocation() - Player->GetActorLocation();
		Player->GetBall()->AddImpulse(-PlayerToTileVector * 20000);

		if (StopVolume) {
			// TODO Re-do the logic. Gamebreaking.
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() +FVector(290,0,0), FRotator(0.f),FActorSpawnParameters());
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 290, 0), FRotator(0.f), FActorSpawnParameters());
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-290, 0, 0), FRotator(0.f), FActorSpawnParameters());
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -290, 0), FRotator(0.f), FActorSpawnParameters());
		}
	}
	else {
		Super::ReactToPlayerOnHit(Player);
	}
}


