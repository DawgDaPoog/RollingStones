// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "OverlapReactor.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
// Sets default values
AOverlapReactor::AOverlapReactor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName("OverlapAll");
	Mesh->SetGenerateOverlapEvents(true);

	Glow = CreateDefaultSubobject<UParticleSystemComponent>(FName("Glow"));
	Glow->SetAutoActivate(true);
	Glow->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AOverlapReactor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOverlapReactor::ReactToPlayer(ARollingStonesBall* Player)
{
}

void AOverlapReactor::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
}

void AOverlapReactor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ReactToPlayer(Cast<ARollingStonesBall>(OtherActor));
	}
	if (OtherActor->ActorHasTag(TEXT("EnemyBall")))
	{
		ReactToEnemyBall(Cast<AEnemyBall>(OtherActor));
	}
}

// Called every frame
void AOverlapReactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBobbing)
	{
		static FVector StartingLocation = GetActorLocation();
		static bool bImGoingUp = true;
		FVector NewLocation = GetActorLocation();
		if (NewLocation.Z > StartingLocation.Z + 17.0f) {
			bImGoingUp = false;
		}
		else if (NewLocation.Z < StartingLocation.Z - 17.0f) {
			bImGoingUp = true;
		}
		float factor = (bImGoingUp ? 25.f : -25.f);
		NewLocation.Z += factor * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

