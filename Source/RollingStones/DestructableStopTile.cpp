// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructableStopTile.h"
#include "RollingStonesBall.h"
#include "Engine/StaticMesh.h"
#include "StopVolume.h"

void ADestructableStopTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
		//TODO add pre-destruction behavior
		Super::ReactToPlayerOnHit(Player);
		Destroy();
}

void ADestructableStopTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	Destroy();
	Player->bIsEmpowered = false;
	FVector PlayerToTileVector = GetActorLocation() - Player->GetActorLocation();
	PlayerToTileVector.Z = 0;
	Player->GetBall()->AddImpulse(PlayerToTileVector * 10000);

	if (StopVolume) {
		float boundry = 50.f;
		float UpperX = GetActorLocation().X + boundry;
		float LowerX = GetActorLocation().X - boundry;
		float UpperY = GetActorLocation().Y + boundry;
		float LowerY = GetActorLocation().Y - boundry;
		FVector PlayerLocation = Player->GetActorLocation();

		if (PlayerLocation.X < UpperX && PlayerLocation.X > LowerX && PlayerLocation.Y < LowerY) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 90, 0), FRotator(0.f), FActorSpawnParameters());
		}
		else if (PlayerLocation.X > UpperX && PlayerLocation.Y > LowerY && PlayerLocation.Y < UpperY) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-90, 0, 0), FRotator(0.f), FActorSpawnParameters());
		}
		else if (PlayerLocation.Y > UpperY && PlayerLocation.X > LowerX && PlayerLocation.X < UpperX) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -90, 0), FRotator(0.f), FActorSpawnParameters());
		}
		else if (PlayerLocation.X < LowerX && PlayerLocation.Y >LowerY && PlayerLocation.Y < UpperY) {
			GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(90, 0, 0), FRotator(0.f), FActorSpawnParameters());
		}
	}
}
