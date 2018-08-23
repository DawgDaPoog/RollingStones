// Fill out your copyright notice in the Description page of Project Settings.

#include "AttractorVolume.h"
#include "Engine/Brush.h"
AAttractorVolume::AAttractorVolume() {
	SetActorScale3D(FVector(0.1, 0.1, 1.f));
	Tags.Add("AttractorVolume");
	
}

