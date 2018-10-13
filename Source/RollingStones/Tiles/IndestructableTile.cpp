// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "IndestructableTile.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Engine/StaticMesh.h"
#include "../VolumesAndBounds/StopVolume.h"

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
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -270, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
		else if (PlayerLocation.X > UpperX && PlayerLocation.Y > LowerY && PlayerLocation.Y < UpperY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(270, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
		else if (PlayerLocation.Y > UpperY && PlayerLocation.X > LowerX && PlayerLocation.X < UpperX) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 270, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
		else if (PlayerLocation.X < LowerX && PlayerLocation.Y >LowerY && PlayerLocation.Y < UpperY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-270, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
	}
}


