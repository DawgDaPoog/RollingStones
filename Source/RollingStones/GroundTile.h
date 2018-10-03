// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundTile.generated.h"

UCLASS()
class ROLLINGSTONES_API AGroundTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGroundTile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ground, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ground;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Highlight, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TopHighlight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bHighlighted = false;

	
	
};
