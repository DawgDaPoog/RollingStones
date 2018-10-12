// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "StopVolume.h"
#include "RollingStonesBall.h"
#include "EnemyBall.h"

AStopVolume::AStopVolume() {
	Tags.Add(FName("StopVolume"));
}

void AStopVolume::SetIntendedActorToStop(FString ActorName)
{
	ActorThatIsIntendedToBeStopped = ActorName;
	UE_LOG(LogTemp, Warning, TEXT("I intend to stop %s"), *ActorName);
}

FString AStopVolume::GetIntededActorToStop()
{
	return ActorThatIsIntendedToBeStopped;
}

void AStopVolume::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->GetName() == ActorThatIsIntendedToBeStopped)
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Stopping the player"));
			ARollingStonesBall* Player = Cast<ARollingStonesBall>(OtherActor);
			Player->ResetMovement();
			Player->bMoving = false;
			Destroy();
		}
		if (OtherActor->ActorHasTag(FName("EnemyBall")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Stopping the enemy"));
			AEnemyBall* EnemyBall = Cast<AEnemyBall>(OtherActor);
			EnemyBall->ResetMovement();
			EnemyBall->InitiateNextStepTimer();
			EnemyBall->EnableMoving();
			Destroy();
		}
		
	}
}
