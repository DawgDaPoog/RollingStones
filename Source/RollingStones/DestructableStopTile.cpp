// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructableStopTile.h"
#include "RollingStonesBall.h"

void ADestructableStopTile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other->ActorHasTag(FName("Player"))) {
		//TODO add pre-destruction behavior
		ReactToPlayerOnHit(Cast<ARollingStonesBall>(Other));
		Destroy();
	}
}
