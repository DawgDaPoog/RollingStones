// Fill out your copyright notice in the Description page of Project Settings.

#include "AttractorGrid.h"
#include "AttractorVolume.h"
#include "Engine/World.h"

// Sets default values
AAttractorGrid::AAttractorGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAttractorGrid::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < GridSize; i++) {
		for (int j = 0; j < GridSize; j++) {
			if (AttractorVolume == nullptr) return;
			AAttractorVolume* Spawned = GetWorld()->SpawnActor<AAttractorVolume>(AttractorVolume,FVector(GetActorLocation().X + -i * GridGap,GetActorLocation().Y + j*GridGap, GetActorLocation().Z),FRotator(),FActorSpawnParameters());
			
		}
	}
}

// Called every frame
void AAttractorGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

