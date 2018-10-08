// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "CameraShakeOnBallCollision.generated.h"

/**
 *  Camera Shake class to activate on player camera when a player hits a tile.
 */
UCLASS()
class ROLLINGSTONES_API UCameraShakeOnBallCollision : public UCameraShake
{
	GENERATED_BODY()
	
	
public:
	UCameraShakeOnBallCollision();
	
};
