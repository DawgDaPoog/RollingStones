// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructableStopTile.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Engine/StaticMesh.h"
#include "../VolumesAndBounds/StopVolume.h"
#include "Projectile.h"
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
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 70, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
		else if (PlayerLocation.X > UpperX && PlayerLocation.Y > LowerY && PlayerLocation.Y < UpperY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-70, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
		else if (PlayerLocation.Y > UpperY && PlayerLocation.X > LowerX && PlayerLocation.X < UpperX) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -70, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
		else if (PlayerLocation.X < LowerX && PlayerLocation.Y >LowerY && PlayerLocation.Y < UpperY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(70, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(Player->GetName());
		}
	}
}

void ADestructableStopTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	//TODO Add pre-destricton sequence
	Super::ReactToEnemyBall(EnemyBall);
	Destroy();
}

void ADestructableStopTile::ReactToProjectile(AProjectile * Projectile)
{
	Destroy();
	Projectile->StartDestroySequence();
}
