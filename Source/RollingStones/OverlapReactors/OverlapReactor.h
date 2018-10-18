// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverlapReactor.generated.h"

/**
*	A base class for all Overlap Reactors such as pickups, switches, goal marks, teleports, etc.
*/
UCLASS()
class ROLLINGSTONES_API AOverlapReactor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverlapReactor();

	/** StaticMesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ParticleSystem", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* Glow;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ReactToPlayer(class ARollingStonesBall* Player);

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool bIsBobbing = false;

	bool bIsSpinning = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};
