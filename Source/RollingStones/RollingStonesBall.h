// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollingStonesBall.generated.h"

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
	class UCameraComponent* Camera;

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


public:
	ARollingStonesBall();
	
	// Movement Variables for outside tiles to check and react to on collision
	bool bMoving = false;
	bool bIsEmpowered = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//Checking overlapping actors to add force, hence aligning to our grid
	void AlignToTheGrid();

	//Reseting movement variables
	void ResetMovement();

	//Set a timer to enable moving again after hitting the tile or volume
	void EnableMovementTimer();
protected:
	
	/** Initialises the movement variables*/
	void StartMovement(bool IsMovingInX, bool IsNegative);
	
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
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	void EnableMovement();

	bool IsAStopTileBeside(FVector Direction);

	float RightMovement = 0.f;
	float UpMovement = 0.f;

	bool bIsCharging = false;
	bool bIsCharged = false;

	//Force that is applied on when moving
	float ForceApply = 1000000;

	//To check for proper grid allignment
	TSet<AActor*> OverlappingActors;

	void CompleteChargeUp();
	void IncreaseChargeUpParticleEffect();

	FTimerHandle ChargeUpParticleTimer;
	
};
