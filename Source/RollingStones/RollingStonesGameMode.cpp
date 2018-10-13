// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RollingStonesGameMode.h"
#include "PlayerAndNpcLogic/RollingStonesBall.h"

ARollingStonesGameMode::ARollingStonesGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARollingStonesBall::StaticClass();
}
