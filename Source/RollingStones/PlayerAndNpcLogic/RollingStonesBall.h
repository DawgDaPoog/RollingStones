// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollingStonesBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBallDelegate);

UCLASS(config=Game)
class ARollingStonesBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class ACameraActor* Camera;

	/** Particle effect for enchanced movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SparkTrail;

	/** Particle effect for enchanced movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* EnchancedSparkTrail;

	/** Particle system for idling */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* IdleGlow;

	/** Particle system for chargeup effect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ChargeUpEffect;

	/** Component to use the Tile Dropping mechanic */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TileDrop, meta = (AllowPrivateAccess = "true"))
	class UTileDropMechanic* TileDropMechanic;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake")
	TSubclassOf<class UCameraShake> MyShake;

	UPROPERTY(EditInstanceOnly)
	class ACameraActor* WholeViewCamera;
public:
	ARollingStonesBall();
	
	// Movement Variables for outside tiles to check and react to on collision
	bool bMoving = false;
	bool bIsEmpowered = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SmoothTheCamera(float DeltaTime);

	UPROPERTY(EditDefaultsOnly)
		float XCameraOffset = 400.f;


	UPROPERTY(EditDefaultsOnly)
		float ZCameraOffset = 500.f;
	virtual void BeginPlay() override;

	//Checking overlapping actors to add force, hence aligning to our grid
	void AlignToTheGrid();

	//Reseting movement variables
	void ResetMovement();

	//Set a timer to enable moving again after hitting the tile or volume
	void EnableMovementTimer();

	//Called when a player is hit by something lethal
	void Die();

	//A function to redirect the player outwards (Used by the Mirror Tile)
	void RedirectBackwards();

	//Functions to redirect the player sideways (Used by the Mirror Tile)
	void RedirectSideways(bool bRedirectRight);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnDeathScreenWidget();

	/** Initialises the movement variables*/
	UFUNCTION(BlueprintCallable)
	void StartMovement(bool IsMovingInX, bool IsNegative);

	FBallDelegate OnMove;
protected:
	
	/** Called for side to side input */
	void MoveRight();
	void MoveLeft();

	/** Called to move ball forwards and backwards */
	void MoveForward();
	void MoveDown();

	/** Charging movement when holding down the button*/
	void StartChargingMovement();

	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class ACameraActor* GetCamera() const { return Camera; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void DecrementTileDropsLeft();

	bool IsMovingInXZ();

	bool IsMovingInYZ();

	bool bInSectionView = false;

	UFUNCTION(BlueprintCallable)
	void Empower();
	
	UFUNCTION(BlueprintPure)
	int32 GetAmountOfEmpowersLeft();

	UFUNCTION(BlueprintCallable)
	void SetAmountOfEmpowersLeft(int32 ValueToSet);

	UFUNCTION(BlueprintPure)
	int32 GetAmountOfTiledropsLeft();

	UFUNCTION(BlueprintCallable)
	void SetAmountOfTiledropsLeft(int32 ValueToSet);

	UFUNCTION(BlueprintPure)
	ACameraActor* GetWholeViewCamera();

	UFUNCTION(BlueprintCallable)
	void SetWholeViewCamera(ACameraActor* CameraToSet);

	UFUNCTION(BlueprintPure)
	bool IsEmpoweredMovementLocked();

	UFUNCTION(BlueprintPure)
	bool IsTileDroppingLocked();

	UFUNCTION(BlueprintCallable)
	void SetEmpoweredMovementLocked(bool Set);

	UFUNCTION(BlueprintCallable)
	void SetTileDroppingLocked(bool Set);
private:
	void EnableMovement();

	void InitiateTileDrop();

	void TryTileDrop();

	template<int32 Index>
	void SetTiledropIndex();

	bool IsAStopTileBeside(FVector Direction);
	
	float RightMovement = 0.f;
	float UpMovement = 0.f;

	bool bIsCharging = false;
	bool bIsCharged = false;

	//Variables to check if we are moving along X or Y axis
	bool bMovingInXZ = false;
	bool bMovingInYZ = false;

	//Player limitations
	bool bIsEmpoweredMovementLocked = true;
	bool bIsTileDroppingLocked = true;

	//Force that is applied on when moving
	float ForceApply = 1000000;

	//To check for proper grid allignment
	TSet<AActor*> OverlappingActors;

	int32 AmountOfEmpowersLeft = 3;

	int32 AmountOfTiledropsLeft = 1;

	void CompleteChargeUp();
	void IncreaseChargeUpParticleEffect();

	FTimerHandle ChargeUpParticleTimer;
	
};

template<int32 Index>
inline void ARollingStonesBall::SetTiledropIndex()
{
	if (TileDropMechanic)
	{
		TileDropMechanic->SetSelectedIndex(Index);
	}
}
