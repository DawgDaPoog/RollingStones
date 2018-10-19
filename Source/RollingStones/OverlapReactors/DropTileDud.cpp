// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "DropTileDud.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"
#include "../Tiles/GroundTile.h"
#include "../Tiles/StopTile.h"
#include "Engine/World.h"

ADropTileDud::ADropTileDud() 
{
	SoftTileMaterial = CreateDefaultSubobject<UMaterial>(TEXT("SoftTileMaterial"));
	HardTileMaterial = CreateDefaultSubobject<UMaterial>(TEXT("HardTileMaterial"));
	IndestructableTileMaterial = CreateDefaultSubobject<UMaterial>(TEXT("IndestructableTileMaterial"));
}

void ADropTileDud::SetIndex(int32 IndexToSet)
{
	DropTileIndex = IndexToSet;

	if (DropTileIndex == 0)
	{
		if (SoftTileMaterial)
		{
			Mesh->SetMaterial(0, SoftTileMaterial);
		}
	}
	else if (DropTileIndex == 1)
		{
			if (HardTileMaterial)
			{
				Mesh->SetMaterial(0, HardTileMaterial);
			}
		}
	else if (DropTileIndex == 2)
	{
		if (HardTileMaterial)
		{
			Mesh->SetMaterial(0, IndestructableTileMaterial);
		}
	}
}

void ADropTileDud::Tick(float DeltaTime)
{
	FallingSpeed += AccelerationSpeed * DeltaTime;
	FVector NewLocation = GetActorLocation() + FVector(0.f, 0.f, -FallingSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}

void ADropTileDud::ReactToPlayer(ARollingStonesBall * Player)
{
	Player->Die();
}

void ADropTileDud::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	EnemyBall->Die();
}

void ADropTileDud::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->ActorHasTag("GroundTile"))
	{
		FTransform TileSpawnTransform = FTransform(FRotator(0.f), OtherActor->GetActorLocation() +FVector(0.f,0.f,100.f), FVector(1.f));
		switch (DropTileIndex)
		{
		case 0: 
			if (!SoftTile) return;
			GetWorld()->SpawnActor<AStopTile>(SoftTile, TileSpawnTransform, FActorSpawnParameters());
			break;
		case 1:
			if (!HardTile) return;
			GetWorld()->SpawnActor<AStopTile>(HardTile, TileSpawnTransform, FActorSpawnParameters());
			break;
		case 2:
			if (!IndestructableTile) return;
			GetWorld()->SpawnActor<AStopTile>(IndestructableTile, TileSpawnTransform, FActorSpawnParameters());
			break;
		}
		
		Destroy();
	}
}
