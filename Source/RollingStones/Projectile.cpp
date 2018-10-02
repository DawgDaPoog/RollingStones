// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "Projectile.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"


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

	float factor = 300.f;
	NewLocation.X += factor * DeltaTime;
	SetActorLocation(NewLocation);

	SetActorRotation(FRotator(GetActorRotation() + FRotator(0.f, 0.f, 25.f)));

}

void AProjectile::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(FName("StopTile")))
	{
		Destroy();
	}
}

void AProjectile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{

}

