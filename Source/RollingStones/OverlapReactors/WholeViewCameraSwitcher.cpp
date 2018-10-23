// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "WholeViewCameraSwitcher.h"
#include "../PlayerAndNpcLogic/RollingStonesBall.h"
#include "Camera/CameraActor.h"
void AWholeViewCameraSwitcher::ReactToPlayer(ARollingStonesBall * Player)
{
	if (RelatedCamera)
	{
		Player->SetWholeViewCamera(RelatedCamera);
		BlendCameraIfInWholeView(Player);
	}
}
