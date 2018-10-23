// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverlapReactors/OverlapReactor.h"
#include "WholeViewCameraSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINGSTONES_API AWholeViewCameraSwitcher : public AOverlapReactor
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
	class ACameraActor* RelatedCamera;

	virtual void ReactToPlayer(class ARollingStonesBall* Player);

public:

	UFUNCTION(BlueprintImplementableEvent)
	void BlendCameraIfInWholeView(ARollingStonesBall* Player);
	
};
