// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "IndestructableTile.h"
#include "RollingStonesBall.h"
#include "Engine/StaticMesh.h"
#include "StopVolume.h"

void AIndestructableTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
		Super::ReactToPlayerOnHit(Player);
}


void AIndestructableTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	FVector PlayerToTileVector = GetActorLocation() - Player->GetActorLocation();
	PlayerToTileVector.Z = 0;
	Player->GetBall()->AddImpulse(-PlayerToTileVector * 10000);
	Player->bIsEmpowered = false;

	if (StopVolume) {
		// Check from which side player is colliding with the tile and spawn a StopVolume appropriately 
		float boundry = 50.f;
		float UpperX = GetActorLocation().X + boundry;
		float LowerX = GetActorLocation().X - boundry;
		float UpperY = GetActorLocation().Y + boundry;
		float LowerY = GetActorLocation().Y - boundry;
		FVector PlayerLocation = Player->GetActorLocation();

		if (PlayerLocation.X < UpperX && PlayerLocation.X > LowerX && PlayerLocation.Y < LowerY) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -290, 0), FRotator(0.f), FActorSpawnParameters());
		}
		else if (PlayerLocation.X > UpperX && PlayerLocation.Y > LowerY && PlayerLocation.Y < UpperY) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(290, 0, 0), FRotator(0.f), FActorSpawnParameters());
		}
		else if (PlayerLocation.Y > UpperY && PlayerLocation.X > LowerX && PlayerLocation.X < UpperX) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 290, 0), FRotator(0.f), FActorSpawnParameters());
		}
		else if (PlayerLocation.X < LowerX && PlayerLocation.Y >LowerY && PlayerLocation.Y < UpperY) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-290, 0, 0), FRotator(0.f), FActorSpawnParameters());
		}
	}
}


