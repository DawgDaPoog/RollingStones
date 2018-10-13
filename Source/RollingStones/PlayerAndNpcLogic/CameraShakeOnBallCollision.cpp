// Copyright Vladyslav Kulinych 2018. All Rights Reserved.

#include "CameraShakeOnBallCollision.h"




UCameraShakeOnBallCollision::UCameraShakeOnBallCollision()
{
	OscillationDuration = 0.1f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(2.0f, 5.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(10.0f, 15.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(2.0f, 5.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(10.0f, 15.0f);
}