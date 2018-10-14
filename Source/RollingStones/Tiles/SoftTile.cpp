// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "SoftTile.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Engine/StaticMesh.h"
#include "../VolumesAndBounds/StopVolume.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"
#include "Projectile.h"

void ASoftTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
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

void ASoftTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	Destroy();
	Player->bIsEmpowered = false;
	FVector PlayerToTileVector = GetActorLocation() - Player->GetActorLocation();
	PlayerToTileVector.Z = 0;
	Player->GetBall()->AddImpulse(PlayerToTileVector * 20000);
}

void ASoftTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	Destroy();
	FVector EnemyBallToTileVector = GetActorLocation() - EnemyBall->GetActorLocation();
	EnemyBallToTileVector.Z = 0;
	EnemyBall->GetBall()->AddImpulse(EnemyBallToTileVector * 10000);

	if (StopVolume) {
		float boundry = 50.f;
		float UpperX = GetActorLocation().X + boundry;
		float LowerX = GetActorLocation().X - boundry;
		float UpperY = GetActorLocation().Y + boundry;
		float LowerY = GetActorLocation().Y - boundry;
		FVector EnemyBallLocation = EnemyBall->GetActorLocation();

		if (EnemyBallLocation.X < UpperX && EnemyBallLocation.X > LowerX && EnemyBallLocation.Y < LowerY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, 70, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(EnemyBall->GetName());
		}
		else if (EnemyBallLocation.X > UpperX && EnemyBallLocation.Y > LowerY && EnemyBallLocation.Y < UpperY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(-70, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(EnemyBall->GetName());
		}
		else if (EnemyBallLocation.Y > UpperY && EnemyBallLocation.X > LowerX && EnemyBallLocation.X < UpperX) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(0, -70, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(EnemyBall->GetName());
		}
		else if (EnemyBallLocation.X < LowerX && EnemyBallLocation.Y >LowerY && EnemyBallLocation.Y < UpperY) {
			AStopVolume* Volume = GetWorld()->SpawnActor<AStopVolume>(StopVolume, GetActorLocation() + FVector(70, 0, 0), FRotator(0.f), FActorSpawnParameters());
			Volume->SetIntendedActorToStop(EnemyBall->GetName());
		}
	}
}

void ASoftTile::ReactToProjectile(AProjectile * Projectile)
{
	if (Projectile->bHasHitSoftTileAlready)
	{
		Destroy(); // TODO add proper destruction sequence
		Projectile->StartDestroySequence();
	}
	else
	{
		Destroy(); // TODO add proper destruction sequence
		Projectile->bHasHitSoftTileAlready = true;
	}
}


