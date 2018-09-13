// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

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


