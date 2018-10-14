// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MirrorTile.h"
#include "SidewaysMirrorTile.generated.h"

UCLASS()
class ROLLINGSTONES_API ASidewaysMirrorTile : public AMirrorTile
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Edge;


public:	
	// Sets default values for this actor's properties
	ASidewaysMirrorTile();

	UPROPERTY(VisibleAnywhere, Category = Volumes)
		class UBoxComponent* LWallInner;

	UPROPERTY(VisibleAnywhere, Category = Volumes)
		class UBoxComponent* LWallOuter;

	UPROPERTY(VisibleAnywhere, Category = Volumes)
		class UBoxComponent* RWallInner;

	UPROPERTY(VisibleAnywhere, Category = Volumes)
		class UBoxComponent* RWallOuter;

	UPROPERTY(VisibleAnywhere, Category = Volumes)
		class UBoxComponent* ProjectileDeflectorWall;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBeginOnLI(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapBeginOnRI(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapBeginDeflect(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapBeginDeflectProjectile(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void ReactToPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEmpoweredPlayerOnHit(ARollingStonesBall* Player) override;

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall) override;

	virtual void ReactToProjectile(class AProjectile* Projectile) override;

private:
	TSet<AActor*> SetOfOverlappingActorsL;

	TSet<AActor*> SetOfOverlappingActorsR;
	
};
