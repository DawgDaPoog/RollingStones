// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StopTile.h"
#include "DestructableStopTile.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API ADestructableStopTile : public AStopTile
{
	GENERATED_BODY()
	
protected:
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	
	
};
