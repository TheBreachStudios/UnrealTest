// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"

// Unreal Engine
#include "UObject/ConstructorHelpers.h"
#include "Code/Public/UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"

// Game Project
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UnrealTestGameState.h"
#include "UnrealTest/Game/UnrealTestPlayerState.h"
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

	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();
	AUnrealTestGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<AUnrealTestGameState>() : NULL;

	EMatchPhase currentGamePhase = gameInstanceSubsystem->GetCurrentMatchPhase();
	switch (currentGamePhase)
	{
	case EMatchPhase::NONE:
		gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::FILLING);
		break;

	case EMatchPhase::FILLING:

		if (GetNumPlayers() == GetMaxPlayerPerSession())
		{
			gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::WAITING);
			GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen", false);
		}
		break;

	case EMatchPhase::WAITING:
		gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::PLAYING);

	case EMatchPhase::PLAYING:
		if (AUnrealTestPlayerState* playerState = Cast<AUnrealTestPlayerState>(NewPlayer->PlayerState))
		{
			int32 teamID = (GetNumPlayers() - 1) % GetPlayersPerTeam();
			UE_LOG(LogTemp, Warning, TEXT("[AUnrealTestGameMode] FILLING Player: %s Tesm: %i"), *NewPlayer->GetName(), teamID);
			playerState->SetTeamID(teamID);
			gameInstanceSubsystem->AddPlayerToTeam(teamID, NewPlayer);
		}
		break;

	default:
		break;
	}

	if (gameState) { 
		gameState->SetPlayerInSession(GetNumPlayers());
		gameState->SetMaxPlayerInSession(GetMaxPlayerPerSession());
		gameState->SetMatchPhase(currentGamePhase);
	}
	
}
#pragma endregion Overrides

#pragma region Functions
void AUnrealTestGameMode::ActorDied(AActor* DeadActor) {
	if (AUnrealTestCharacter* deadActor = Cast<AUnrealTestCharacter>(DeadActor)) {
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Emerald,
			FString::Printf(TEXT("[AUnrealTestGameMode][LocalRole: %s][RemoteRole: %s] Actor died %s"), *UEnum::GetValueAsString(GetLocalRole()), *UEnum::GetValueAsString(GetRemoteRole()), *DeadActor->GetName()));
	}
}
#pragma endregion Functions