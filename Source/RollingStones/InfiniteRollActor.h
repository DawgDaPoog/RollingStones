// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfiniteRollActor.generated.h"

UCLASS()
class ROLLINGSTONES_API AInfiniteRollActor : public AActor
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly, Category = "Ground Box")
	class UBoxComponent* GroundBox;

	UPROPERTY(EditDefaultsOnly, Category = "MoveTrigger")
	class UBoxComponent* MoveTrigger;


public:	
	// Sets default values for this actor's properties
	AInfiniteRollActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapMoveActor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
