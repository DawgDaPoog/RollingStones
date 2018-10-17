// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "SidewaysMirrorTile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Projectile.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "../PlayerAndNpcLogic/EnemyBall.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
// Sets default values
ASidewaysMirrorTile::ASidewaysMirrorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Edge = CreateDefaultSubobject<UStaticMeshComponent>(FName("Edge"));
	Edge->SetupAttachment(RootComponent);

	LWallInner = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerLeftWall"));
	LWallInner->SetupAttachment(RootComponent);
	LWallInner->SetCollisionProfileName("BlockAllDynamic");
	LWallInner->SetGenerateOverlapEvents(true);


	RWallInner = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerRightWall"));
	RWallInner->SetupAttachment(RootComponent);
	RWallInner->SetCollisionProfileName("BlockAllDynamic");
	RWallInner->SetGenerateOverlapEvents(true);


	ProjectileDeflectorWall = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileDeflectorWall"));
	ProjectileDeflectorWall->SetupAttachment(RootComponent);
	ProjectileDeflectorWall->SetCollisionProfileName("OverlapAll");
	ProjectileDeflectorWall->SetGenerateOverlapEvents(true);
	

	RWallInner->OnComponentHit.AddDynamic(this, &ASidewaysMirrorTile::OnHitRI);

	LWallInner->OnComponentHit.AddDynamic(this, &ASidewaysMirrorTile::OnHitLI);

	//Binding functions to when something overlaps with our Box Components
	//LWallInner->OnComponentBeginOverlap.AddDynamic(this, &ASidewaysMirrorTile::OnOverlapBeginOnLI);

	//RWallInner->OnComponentBeginOverlap.AddDynamic(this, &ASidewaysMirrorTile::OnOverlapBeginOnRI);

	RWallInner->OnComponentBeginOverlap.AddDynamic(this, &ASidewaysMirrorTile::OnOverlapBeginDeflect);

	LWallInner->OnComponentBeginOverlap.AddDynamic(this, &ASidewaysMirrorTile::OnOverlapBeginDeflect);

	ProjectileDeflectorWall->OnComponentBeginOverlap.AddDynamic(this, &ASidewaysMirrorTile::OnOverlapBeginDeflectProjectile);
}

// Called when the game starts or when spawned
void ASidewaysMirrorTile::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%s: I have rotation %f"), *GetName(),GetActorRotation().Yaw);
}

// Called every frame
void ASidewaysMirrorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASidewaysMirrorTile::OnHitRI(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		TSet<AActor*> OverlappingActors;
		ProjectileDeflectorWall->GetOverlappingActors(OverlappingActors);
		bool bDoesItOverlap = false;
		for (auto Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("Player"))
			{
				bDoesItOverlap = true;
			}
		}
		if (bDoesItOverlap)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting sideways"));
			Cast<ARollingStonesBall>(OtherActor)->RedirectSideways(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting backwards"));
			Cast<ARollingStonesBall>(OtherActor)->RedirectBackwards();
		}
	}
	if (OtherActor->ActorHasTag("EnemyBall"))
	{
		TSet<AActor*> OverlappingActors;
		ProjectileDeflectorWall->GetOverlappingActors(OverlappingActors);
		bool bDoesItOverlap = false;
		for (auto Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("EnemyBall"))
			{
				bDoesItOverlap = true;
			}
		}
		if (bDoesItOverlap)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting sideways"));
			Cast<AEnemyBall>(OtherActor)->RedirectSideways(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting backwards"));
			Cast<AEnemyBall>(OtherActor)->RedirectBackwards();
		}
	}
}

void ASidewaysMirrorTile::OnHitLI(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		TSet<AActor*> OverlappingActors;
		ProjectileDeflectorWall->GetOverlappingActors(OverlappingActors);
		bool bDoesItOverlap = false;
		for (auto Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("Player"))
			{
				bDoesItOverlap = true;
			}
		}
		if (bDoesItOverlap)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting sideways"));
			Cast<ARollingStonesBall>(OtherActor)->RedirectSideways(false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting backwards"));
			Cast<ARollingStonesBall>(OtherActor)->RedirectBackwards();
		}
	}
	if (OtherActor->ActorHasTag("EnemyBall"))
	{
		TSet<AActor*> OverlappingActors;
		ProjectileDeflectorWall->GetOverlappingActors(OverlappingActors);
		bool bDoesItOverlap = false;
		for (auto Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("EnemyBall"))
			{
				bDoesItOverlap = true;
			}
		}
		if (bDoesItOverlap)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting sideways"));
			Cast<AEnemyBall>(OtherActor)->RedirectSideways(false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit on Right Wall registered, redirecting backwards"));
			Cast<AEnemyBall>(OtherActor)->RedirectBackwards();
		}
	}
}

void ASidewaysMirrorTile::OnHitDeflect(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit on Outer Wall registered"));
		Cast<ARollingStonesBall>(OtherActor)->RedirectBackwards();
	}
}

void ASidewaysMirrorTile::OnOverlapBeginDeflect(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Redirecting"));
		Cast<ARollingStonesBall>(OtherActor)->RedirectBackwards();
	}
	if (OtherActor->ActorHasTag(FName("EnemyBall")))
	{
		Cast<AEnemyBall>(OtherActor)->RedirectBackwards();
	}
	if (OtherActor->ActorHasTag(FName("Projectile")))
	{
		ReactToProjectile(Cast<AProjectile>(OtherActor));
	}
}

void ASidewaysMirrorTile::OnOverlapBeginDeflectProjectile(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Projectile")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped w/ projectile"));
		FVector WallForwardVector = ProjectileDeflectorWall->GetForwardVector();
		FVector ProjectileForwardVector = OtherActor->GetActorForwardVector();

		float XFactor;
		float YFactor;

		AProjectile* Projectile = Cast<AProjectile>(OtherActor);
		FTransform ProjectileTransform = Projectile->GetTransform();
		Projectile->GetXYFactors(XFactor, YFactor);

		if (GetActorRotation().Yaw > -10.f && GetActorRotation().Yaw < 10.f)
		{
		
			if (XFactor < 0.f && (abs(XFactor) > abs(YFactor)))
			{
				
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw - 90.f, 0.f));
			}
			else
			{
				
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw + 90.f, 0.f));
			}
			Swap(XFactor, YFactor);
			YFactor = -YFactor;
			XFactor = -XFactor;
		}
		if (GetActorRotation().Yaw > 80.f && GetActorRotation().Yaw < 100.f)
		{
		
			if (XFactor > 0.f && (abs(XFactor) > abs(YFactor)))
			{
				
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw + 90.f, 0.f));
			}
			else
			{
				
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw - 90.f, 0.f));
			}
			Swap(XFactor, YFactor);
		}
		if ((GetActorRotation().Yaw < -170.f && GetActorRotation().Yaw > -180.f) || (GetActorRotation().Yaw > 170.f && GetActorRotation().Yaw < 180.f))
		{
		
			if (YFactor > 0.f && (abs(YFactor) > abs(XFactor)))
			{
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw + 90.f, 0.f));
			}
			else
			{
				
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw - 90.f, 0.f));
			}
			Swap(XFactor, YFactor);
			XFactor = -XFactor;
			YFactor = -YFactor;
		}
		if (GetActorRotation().Yaw < -80.f && GetActorRotation().Yaw > -100.f)
		{
			
			if (YFactor > 0.f && (abs(YFactor) > abs(XFactor)))
			{
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw - 90.f, 0.f));
			}
			else
			{
				
				Projectile->SetActorRotation(FRotator(0.f, ProjectileTransform.GetRotation().Rotator().Yaw + 90.f, 0.f));
			}
			Swap(XFactor, YFactor);
		}
	
		Projectile->SetActorLocation(ProjectileDeflectorWall->GetComponentLocation());
		Projectile->SetXYFactors(XFactor, YFactor);
	}
}

void ASidewaysMirrorTile::ReactToPlayerOnHit(ARollingStonesBall * Player)
{
}

void ASidewaysMirrorTile::ReactToEmpoweredPlayerOnHit(ARollingStonesBall * Player)
{
}

void ASidewaysMirrorTile::ReactToEnemyBall(AEnemyBall * EnemyBall)
{
}

void ASidewaysMirrorTile::ReactToProjectile(AProjectile * Projectile)
{
}