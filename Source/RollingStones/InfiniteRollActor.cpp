// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "InfiniteRollActor.h"
#include "Components/BoxComponent.h"



// Sets default values
AInfiniteRollActor::AInfiniteRollActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GroundBox = CreateDefaultSubobject<UBoxComponent>(FName("GroundBox"));
	GroundBox->SetCollisionProfileName(FName("BlockAllDynamic"));
	SetRootComponent(GroundBox);

	MoveTrigger = CreateDefaultSubobject<UBoxComponent>(FName("MoveTrigger"));
	MoveTrigger->SetCollisionProfileName(FName("OverlapAll"));
	MoveTrigger->SetupAttachment(RootComponent);

	MoveTrigger->OnComponentBeginOverlap.AddDynamic(this, &AInfiniteRollActor::OnOverlapMoveActor);
}

// Called when the game starts or when spawned
void AInfiniteRollActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInfiniteRollActor::OnOverlapMoveActor(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		FVector RightVec = GetActorRightVector();
	
		SetActorLocation(GetActorLocation() + RightVec *3900.f - FVector(0.f,0.f,2.f));
	}
}

// Called every frame
void AInfiniteRollActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

