// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"

// Unreal Engine
#include "UObject/ConstructorHelpers.h"
#include "Code/Public/UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Game Project
#include "UnrealTest/Controller/UnrealTestPlayerController.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UnrealTestGameState.h"
#include "UnrealTest/Game/UnrealTestPlayerState.h"
#include "UnrealTest/Game/UnrealTestGameInstance.h"
#include "UnrealTest/UI/UnrealTestHUD.h"
#include "UnrealTest/Data/SChampionDefinition.h"
#include <OnlineSubsystemUtils.h>

#pragma region Initialization
// Constructor
AUnrealTestGameMode::AUnrealTestGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set Player Controller class 
	PlayerControllerClass = AUnrealTestPlayerController::StaticClass();
}
#pragma endregion Initialization

#pragma region Overrides
// On pre login event.
void AUnrealTestGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMe)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMe);
}

// On login event.
APlayerController* AUnrealTestGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	// Get champ passed through connection URL
	AUnrealTestPlayerController* newPlayerController = Cast<AUnrealTestPlayerController>(Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage));
	static const FString ContextString(TEXT("Champion"));
	FString champOption = UGameplayStatics::ParseOption(Options, "champ");

	// If there is a champ setted try resetting pawn use by the newPlayerController
	if (champOption != "") {
		FChampionDefinition* Champion = ChampionsDataTable->FindRow<FChampionDefinition>(FName(champOption), ContextString, true);

		if (Champion)
		{
			newPlayerController->Server_SetPawn(Champion->ChampionClass, FName(champOption));
		}
	}

	return newPlayerController;
}

// On post login event.
void AUnrealTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	
	UUnrealTestGameInstance* gameInstance = Cast<UUnrealTestGameInstance>(NewPlayer->GetGameInstance());
	UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();
	AUnrealTestGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<AUnrealTestGameState>() : NULL;

	// If pawn is Server, reset pawn with gameInstance info
	if (gameInstance->DefaultPawn)
	{
		AUnrealTestPlayerController* newPlayerController = Cast<AUnrealTestPlayerController>(NewPlayer);
		newPlayerController->Server_SetPawn(gameInstance->DefaultPawn, gameInstance->DefaultPawnName);
	}

	// Check current phase
	EMatchPhase currentGamePhase = gameInstanceSubsystem->GetCurrentMatchPhase();
	switch (currentGamePhase)
	{
	// If NONE is the first player login. Change to FILLING.
	case EMatchPhase::NONE:
		gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::FILLING);
		break;

	// if FILLING check if MaxPlayerPerssion has been reached. If so, server travel to MatchLevel
	case EMatchPhase::FILLING:

		if (GetNumPlayers() == GetMaxPlayerPerSession())
		{
			gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::WAITING);
			GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/MatchLevel?listen", false);
		}
		break;
	
	// if WAITING is the first player to arrive Match Level. Change to PLAYING
	case EMatchPhase::WAITING:
		gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::PLAYING);

		// TODO: Assumes that only exists two teams. A new implementatio will be need for more than two teams
		gameInstanceSubsystem->OnLastTeamPlayerDied.AddDynamic(this, &AUnrealTestGameMode::OnGameOverEvent);

		// DO NOT BREAK EXECUTION ON WAITING TO PLAYING
	// if PLAYING add player to team and update player state
	case EMatchPhase::PLAYING:
		if (AUnrealTestPlayerState* playerState = Cast<AUnrealTestPlayerState>(NewPlayer->PlayerState))
		{
			int32 teamID = (GetNumPlayers() - 1) % GetPlayersPerTeam();
			gameInstanceSubsystem->AddPlayerToTeam(teamID, NewPlayer);
			playerState->SetTeamID(teamID);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("[AUnrealTestGameMode] No player state"));
		}
		break;

	default:
		break;
	}

	// Update Game state
	if (gameState) { 
		gameState->SetPlayerInSession(GetNumPlayers());
		gameState->SetMaxPlayerInSession(GetMaxPlayerPerSession());
		gameState->SetMatchPhase(currentGamePhase);
	}
	
}

// Overrides default pawn class.
UClass* AUnrealTestGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UClass* defaultPawnClass = Super::GetDefaultPawnClassForController_Implementation(InController);

	
	// Try to get the player pawn set on AUnrealTestPlayerController
	AUnrealTestPlayerController* PlayerController = Cast<AUnrealTestPlayerController>(InController);
	if (PlayerController && PlayerController->GetPlayerPawnClass())
	{
		defaultPawnClass = PlayerController->GetPlayerPawnClass();
	}

	return defaultPawnClass;

}
#pragma endregion Overrides

#pragma region Functions
// On any actor died.
void AUnrealTestGameMode::ActorDied(AActor* DeadActor) {
	if (AUnrealTestCharacter* deadActor = Cast<AUnrealTestCharacter>(DeadActor)) {

			UGameInstance* gameInstance = GetWorld()->GetGameInstance();
			UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();
			EMatchPhase currentGamePhase = gameInstanceSubsystem->GetCurrentMatchPhase();

			// If Current game phase is not PLAYING respawn character
			switch (currentGamePhase)
			{
			case EMatchPhase::NONE:
			case EMatchPhase::FILLING:
			case EMatchPhase::WAITING:
				if (AUnrealTestCharacter* deadPlayer = Cast<AUnrealTestCharacter>(DeadActor))
				{
					deadPlayer->RespawnPlayer();
				}
				break;
			// if PLAYING update on dead status on game instance to handle defeat conditions
			case EMatchPhase::PLAYING:
				if (AUnrealTestCharacter* deadPlayer = Cast<AUnrealTestCharacter>(DeadActor))
				{
					deadPlayer->Multicast_Die();
					AUnrealTestPlayerState* damagerPlayerState = Cast<AUnrealTestPlayerState>(deadPlayer->GetPlayerState());
					gameInstanceSubsystem->OnPlayerDied(damagerPlayerState->GetTeamID());
				}
				break;
			default:
				break;
			}
	}
}

// Checks if player can damage another.
void AUnrealTestGameMode::ProcessDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	AUnrealTestCharacter* damagedPlayer = Cast<AUnrealTestCharacter>(DamagedActor);
	AUnrealTestCharacter* damagerPlayer = Cast<AUnrealTestCharacter>(EventInstigator->GetPawn());

	if (damagedPlayer && damagerPlayer) {
		AUnrealTestPlayerState* damagedPlayerState = Cast<AUnrealTestPlayerState>(damagedPlayer->GetPlayerState());
		AUnrealTestPlayerState* damagerPlayerState = Cast<AUnrealTestPlayerState>(damagerPlayer->GetPlayerState());

		// If players are differente and have the different TeamID or their team ID is -1 (i.e.: they are in the waiting room) they can receive damage
		if (
			(
				damagedPlayerState->GetTeamID() == damagerPlayerState->GetTeamID()
				&& (damagedPlayerState->GetTeamID() != -1)
				&& (damagerPlayerState->GetTeamID() != -1)
			)
			|| damagerPlayer == damagedPlayer
		)
		{
			return;
		}
	}

	UGameplayStatics::ApplyDamage(DamagedActor, BaseDamage, EventInstigator, DamageCauser, DamageTypeClass);
}

// On game over event.
void AUnrealTestGameMode::OnGameOverEvent(int32 TeamID)
{
	OnGameOver.Broadcast(TeamID);
}
#pragma endregion Functions