// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "Teleporter.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"
#include "Components/StaticMeshComponent.h"

ATeleporter::ATeleporter()
{
	ExitTeleporter = CreateDefaultSubobject<AActor>(FName("Teleporter"));
}

void ATeleporter::ReactToPlayer(ARollingStonesBall * Player)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Detected Player Begin Overlap "), *GetName());

	if (ExitingActorNames.Contains(Player->GetName())) 
	{
		return; 
	}
	else
	{
		if (ExitTeleporter)
		{
			Cast<ATeleporter>(ExitTeleporter)->ExitingActorNames.Push(Player->GetName());
			Player->GetBall()->SetConstraintMode(EDOFMode::None);
			Player->SetActorLocation(ExitTeleporter->GetActorLocation());
			if(Player->IsMovingInXZ())
			{ 
				Player->GetBall()->SetConstraintMode(EDOFMode::XZPlane); 
			}
			else if (Player->IsMovingInXZ()) 
			{ 
				Player->GetBall()->SetConstraintMode(EDOFMode::XZPlane); 
			}
		}
	}
}

void ATeleporter::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
	if (ExitingActorNames.Contains(EnemyBall->GetName())) 
	{ 
		return; 
	}
	else
	{
		if (ExitTeleporter)
		{
			Cast<ATeleporter>(ExitTeleporter)->ExitingActorNames.Add(EnemyBall->GetName());
			EnemyBall->GetBall()->SetConstraintMode(EDOFMode::None);
			EnemyBall->SetActorLocation(ExitTeleporter->GetActorLocation());
			if (EnemyBall->IsMovingInXZ())
			{
				EnemyBall->GetBall()->SetConstraintMode(EDOFMode::XZPlane);
			}
			if (EnemyBall->IsMovingInYZ())
			{
				EnemyBall->GetBall()->SetConstraintMode(EDOFMode::YZPlane);
			}
		}
	}
}

void ATeleporter::NotifyActorEndOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Detected player end overlap"), *GetName());
	}

	
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		auto Player = Cast<ARollingStonesBall>(OtherActor);
		if (ExitingActorNames.Contains(Player->GetName()))
		{
			ExitingActorNames.Remove(Player->GetName());
		}
	}
	if (OtherActor->ActorHasTag(FName("EnemyBall")))
	{
		auto EnemyBall = Cast<AEnemyBall>(OtherActor);
		if (ExitingActorNames.Contains(EnemyBall->GetName()))
		{
			ExitingActorNames.Remove(EnemyBall->GetName());
		}
	}
}
