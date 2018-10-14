// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "MirrorInnerWallComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API UMirrorInnerWallComponent : public UBoxComponent
{
	GENERATED_BODY()

	UMirrorInnerWallComponent();

private:


public:
	bool bIsItOnTheRight = false;
	
	
};
