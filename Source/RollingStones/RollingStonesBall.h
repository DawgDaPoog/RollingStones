// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

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

public:
	ARollingStonesBall();
	
	bool bMoving = false;


	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	

	/** Called for side to side input */
	void MoveRight();

	/** Called to move ball forwards and backwards */
	void MoveForward();

	void MoveDown();

	void MoveLeft();

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
	float RightMovement = 0.f;
	float UpMovement = 0.f;
	float ForceApply = 1000000;

	void ResetMovement();
};
