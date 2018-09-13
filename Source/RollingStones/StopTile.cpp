
// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "StopTile.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "RollingStonesBall.h"

// Sets default values
AStopTile::AStopTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetNotifyRigidBodyCollision(true);
	RootComponent = Mesh;

	Tags.Add(FName("StopTile"));
}

// Called when the game starts or when spawned
void AStopTile::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AStopTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStopTile::ReactToPlayerOnHit(ARollingStonesBall* Player)
{
		Player->bMoving = false;
		Player->ResetMovement();
}

void AStopTile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other->ActorHasTag(FName("Player")))
	{
		ReactToPlayerOnHit(Cast<ARollingStonesBall>(Other));
	}
}




