// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "KillerTile.h"
#include "RollingStonesBall.h"

void AKillerTile::BeginPlay()
{
	Super::BeginPlay();

	
	StartingLocation = GetActorLocation();
}

AKillerTile::AKillerTile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKillerTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FVector NewLocation = GetActorLocation();
	if (NewLocation.Z > StartingLocation.Z + 15.0f) {
		bImGoingUp = false;
	}
	else if (NewLocation.Z < StartingLocation.Z - 15.0f) {
		bImGoingUp = true;
	}
	float factor = (bImGoingUp ? 20.f : -20.f);
	NewLocation.Z += factor * DeltaTime;
	SetActorLocation(NewLocation);
}



void AKillerTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
	Player->Destroy();
}

void AKillerTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
	Player->Destroy();
}




