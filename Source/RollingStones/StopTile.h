// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StopTile.generated.h"


class ARollingStonesBall;

class AStopVolume;

/**
* A Basic tile class with overridable functionality. Not meant to be placed in the world. It is meant to be a parent class only for other tiles.
*/
UCLASS()
class ROLLINGSTONES_API AStopTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStopTile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category = SpawningCollision, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AStopVolume> StopVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	class UParticleSystemComponent* Glow;

	UPROPERTY(EditDefaultsOnly, Category = "Destruction")
	class UParticleSystemComponent* OnDestructionParticleEffects;

	//Array of actors to spawn during the tile destruction (if it can be destroyed)
	UPROPERTY(EditDefaultsOnly, Category = "Destruction")
	TArray<TSubclassOf<AActor>> ArrayOfActorsToSpawnOnDestruction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player);

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player);

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

};
