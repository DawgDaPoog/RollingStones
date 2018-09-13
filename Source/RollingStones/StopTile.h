// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StopTile.generated.h"


class ARollingStonesBall;

class AStopVolume;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};