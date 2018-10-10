// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "Projectile.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"
#include "RollingStonesBall.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(FName("Projectile"));
	SetRootComponent(Projectile);

	FieryPath = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FieryPath"));
	FieryPath->bAutoActivate = true;
	FieryPath->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	

	FVector NewLocation = GetActorLocation();

	NewLocation.X += XFactor * DeltaTime;
	NewLocation.Y += YFactor * DeltaTime;

	SetActorLocation(NewLocation);

	SetActorRotation(FRotator(GetActorRotation() + FRotator(0.f, 0.f, 25.f)));

}

void AProjectile::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(FName("StopTile")))
	{
		Destroy();
	}
	if (OtherActor->ActorHasTag("Player"))
	{
		if (Cast<ARollingStonesBall>(OtherActor)->bIsEmpowered)
		{
			Destroy();
		}
		else
		{
			Cast<ARollingStonesBall>(OtherActor)->Die(); // TODO add proper player destroyed sequence
			Destroy();
		}
	}
}

void AProjectile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{

}

void AProjectile::SetXYFactors(float XFactorToSet, float YFactorToSet)
{
		XFactor = XFactorToSet;
		YFactor = YFactorToSet;
}

