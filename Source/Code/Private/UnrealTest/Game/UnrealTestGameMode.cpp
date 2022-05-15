// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"

// Unreal Engine
#include "UObject/ConstructorHelpers.h"
#include "Code/Public/UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"

// Game Project
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UnrealTestGameState.h"
#include <OnlineSubsystemUtils.h>

#pragma region Initialization
AUnrealTestGameMode::AUnrealTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter"));
	//bUseSeamlessTravel = true;
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
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();

	switch (gameInstanceSubsystem->GamePhase)
	{
	case EMatchPhase::NONE:
		gameInstanceSubsystem->GamePhase = EMatchPhase::FILLING;
		break;

	case EMatchPhase::FILLING:

		if (GetNumPlayers() == GetMaxPlayerPerSession())
		{
			gameInstanceSubsystem->GamePhase = EMatchPhase::WAITING;
			GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen", false);
		}
		break;

	case EMatchPhase::WAITING:
		gameInstanceSubsystem->GamePhase = EMatchPhase::PLAYING;
		break;

	case EMatchPhase::PLAYING:
		break;

	default:
		break;
	}

	if (gameState) { 
		gameState->SetPlayerInSession(GetNumPlayers()); 
		gameState->SetMaxPlayerInSession(GetMaxPlayerPerSession()); 
		gameState->SetMatchPhase(gameInstanceSubsystem->GamePhase); 
	}
	
}
#pragma endregion Overrides
