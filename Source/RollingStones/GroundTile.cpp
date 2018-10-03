// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "GroundTile.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGroundTile::AGroundTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ground = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ground"));
	SetRootComponent(Ground);

	TopHighlight = CreateDefaultSubobject<UStaticMeshComponent>(FName("TopHighlight"));
	TopHighlight->SetupAttachment(Ground);
	TopHighlight->SetCollisionProfileName(FName("OverlapAll"));
	TopHighlight->bVisible = false;

	Tags.Add(FName("GroundTile"));
}

// Called when the game starts or when spawned
void AGroundTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bHighlighted) {
		TopHighlight->SetVisibility(true);
		bHighlighted = false;
	}
	else {
		TopHighlight->SetVisibility(false);
	}
}

