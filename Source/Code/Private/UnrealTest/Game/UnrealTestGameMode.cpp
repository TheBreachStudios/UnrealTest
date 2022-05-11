// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealTestGameMode::AUnrealTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
