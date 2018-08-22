// Fill out your copyright notice in the Description page of Project Settings.

#include "StopTile.h"
#include "RollingStonesBall.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
AStopTile::AStopTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetNotifyRigidBodyCollision(true);
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AStopTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStopTile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);


	if (Other->ActorHasTag(FName("Player")))
	Cast<ARollingStonesBall>(Other)->bMoving = false;

}

// Called every frame
void AStopTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



