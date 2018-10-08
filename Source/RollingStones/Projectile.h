// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


/**
* Projectile that is shot from Projectile Tile and Shooter Tiles
*/
UCLASS()
class ROLLINGSTONES_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	/** StaticMesh used for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ParticleSystem, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* FieryPath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void SetXYFactors(float XFactorToSet, float YFactorToSet);
private:
	//Default Speed setting on spawn
	float XFactor = 0.f;
	float YFactor = 0.f;
};
