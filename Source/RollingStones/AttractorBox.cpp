// Fill out your copyright notice in the Description page of Project Settings.

#include "AttractorBox.h"
#include "Components/BoxComponent.h"


AAttractorBox::AAttractorBox() {
	Tags.Add(FName("AttractorVolume"));

	WallUp = CreateDefaultSubobject<UBoxComponent>(TEXT("WallUp"));
	WallUp->SetupAttachment(RootComponent);
	WallUp->bAutoActivate = true;

	WallDown = CreateDefaultSubobject<UBoxComponent>(TEXT("WallDown"));
	WallDown->SetupAttachment(RootComponent);
	WallDown->bAutoActivate = true;

	WallLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("WallLeft"));
	WallLeft->SetupAttachment(RootComponent);
	WallLeft->bAutoActivate = true;

	WallRight = CreateDefaultSubobject<UBoxComponent>(TEXT("WallRight"));
	WallRight->SetupAttachment(RootComponent);
	WallRight->bAutoActivate = true;

}

void AAttractorBox::LockWalls()
{
	
	WallUp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	
	WallDown->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	WallLeft->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	WallRight->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AAttractorBox::UnlockWalls()
{
	WallUp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	WallDown->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	WallLeft->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	WallRight->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}
