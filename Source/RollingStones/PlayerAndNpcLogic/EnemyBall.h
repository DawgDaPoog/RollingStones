// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBall.generated.h"


UENUM(BlueprintType)
enum class EMovementOrder : uint8
{
	Up    UMETA(DisplayName = "Move Up"),
	Right      UMETA(DisplayName = "Move Right"),
	Left	UMETA(DisplayName = "Move Left"),
	Down	UMETA(DisplayName = "Move Down"),
};

UCLASS()
class ROLLINGSTONES_API AEnemyBall : public AActor
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** StaticMesh used for the the arrow indication */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Arrow;

	/** Particle system for idling */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* IdleGlow;

	/** Particle system for dying */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ExplosionOnDeath;

public:
	// Sets default values for this pawn's properties
	AEnemyBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	
	//Called when a ball is hit by something lethal
	void Die();

	//Reseting movement variables
	void ResetMovement();

	void EnableMoving();

	//Checking overlapping actors to add force, hence aligning to our grid
	void AlignToTheGrid();

	// Movement Variables for outside tiles to check and react to on collision
	bool bMoving = false;

	//Starts the timer for excecuting the next step movement function
	void InitiateNextStepTimer();

	//A function to redirect the player outwards (Used by the Mirror Tile)
	void RedirectBackwards();

	//Functions to redirect the player sideways (Used by the Mirror Tile)
	void RedirectSideways(bool bRedirectRight);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool IsMovingInXZ();

	bool IsMovingInYZ();

private:

	// Checks the next movement order and excecutes it
	void InitiateNextStepMovement();

	void IterateTheOrder();

	/** Initialises the movement variables*/
	void StartMovement(bool IsMovingInX, bool IsNegative);

	/**A method to turn the guidence arrow to where the next move will try to commence*/
	void RotateTheArrow();

	/**A method to make sure the arrow is constantly on top of the ball*/
	void PositionTheArrow();

	//Force that is applied on when moving
	float ForceApply = 1000000;

	//Floats to determine where the ball moves
	float RightMovement = 0.f;
	float UpMovement = 0.f;
	
	//Checks if a stop tile is where the ball intends to move
	bool IsAStopTileBeside(FVector Direction);

	//To check for proper grid allignment
	TSet<AActor*> OverlappingActors;

	// Timer handle for initialising next move
	struct FTimerHandle NextMoveTimer;

	// Iterator to show which step we are excecuting
	int MovementIterator = 0;

	//variables to check if we are moving along X axis or Y axis
	bool bMovingInXZ = false;
	bool bMovingInYZ = false;

	// Array of moving orders for designer to choose the movement pattern of this ball
	UPROPERTY(EditInstanceOnly, Category = "Movement Orders")
	TArray<EMovementOrder> MovementOrders;
	
};
