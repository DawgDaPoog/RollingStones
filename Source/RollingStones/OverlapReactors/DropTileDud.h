// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "DropTileDud.generated.h"

/**
 * 
 */

class AStopTile;
UCLASS()
class ROLLINGSTONES_API ADropTileDud : public AOverlapReactor
{
	GENERATED_BODY()

	ADropTileDud();

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	class UMaterial* SoftTileMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	class UMaterial* HardTileMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	class UMaterial* IndestructableTileMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "TileDrops")
	TSubclassOf<AStopTile> SoftTile;

	UPROPERTY(EditDefaultsOnly, Category = "TileDrops")
	TSubclassOf<AStopTile> HardTile;

	UPROPERTY(EditDefaultsOnly, Category = "TileDrops")
	TSubclassOf<AStopTile> IndestructableTile;
public:
	void SetIndex(int32 IndexToSet);
private:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	float FallingSpeed = 50.f;
	
	UPROPERTY(EditDefaultsOnly)
	float AccelerationSpeed = 50.f;

	int32 DropTileIndex = 0;

	virtual void ReactToPlayer(class ARollingStonesBall* Player);

	virtual void ReactToEnemyBall(class AEnemyBall* EnemyBall);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	
};
