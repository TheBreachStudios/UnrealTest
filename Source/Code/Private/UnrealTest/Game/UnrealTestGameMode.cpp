// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"

// Unreal Engine
#include "UObject/ConstructorHelpers.h"

// Game Project
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UnrealTestGameState.h"

#pragma region Initialization
AUnrealTestGameMode::AUnrealTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
#pragma endregion Initialization

#pragma region Overrides
// Overrides

// On post login event;
void AUnrealTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUnrealTestGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<AUnrealTestGameState>() : NULL;
	if (gameState) { gameState->SetPlayerInSession(GetNumPlayers()); }
	if (gameState) { gameState->SetMaxPlayerInSession(GetMaxPlayerPerSession()); }
}
#pragma endregion Overrides
