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
// Overrides

// On pre login event
void AUnrealTestGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMe)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMe);
	UE_LOG(LogTemp, Warning, TEXT("AUnrealTestGameMode::PreLogin %s"), *Options);
}

// On login event
APlayerController* AUnrealTestGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	AUnrealTestPlayerController* newPlayerController = Cast<AUnrealTestPlayerController>(Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage));
	static const FString ContextString(TEXT("Champion"));
	FString champOption = UGameplayStatics::ParseOption(Options, "champ");

	if (champOption != "") {
		FChampionDefinition* Champion = ChampionsDataTable->FindRow<FChampionDefinition>(FName(champOption), ContextString, true);
		newPlayerController->Server_SetPawn(Champion->ChampionClass, FName(champOption));
		UE_LOG(LogTemp, Warning, TEXT("AUnrealTestGameMode::Login %s"), *Options);
	}

	return newPlayerController;
}

// On post login event;
void AUnrealTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	
	UUnrealTestGameInstance* gameInstance = Cast<UUnrealTestGameInstance>(NewPlayer->GetGameInstance());
	UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();
	AUnrealTestGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<AUnrealTestGameState>() : NULL;

	if (gameInstance->DefaultPawn)
	{
		AUnrealTestPlayerController* newPlayerController = Cast<AUnrealTestPlayerController>(NewPlayer);
		newPlayerController->Server_SetPawn(gameInstance->DefaultPawn, gameInstance->DefaultPawnName);
	}

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
			GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/WaitingRoom?listen", false);
		}
		break;

	case EMatchPhase::WAITING:
		gameInstanceSubsystem->SetCurrentMatchPhase(EMatchPhase::PLAYING);

		// TODO: Assumes that only exists two teams. A new implementatio will be need for more than two teams
		gameInstanceSubsystem->OnLastTeamPlayerDied.AddDynamic(this, &AUnrealTestGameMode::OnGameOverEvent);

	case EMatchPhase::PLAYING:
		if (AUnrealTestPlayerState* playerState = Cast<AUnrealTestPlayerState>(NewPlayer->PlayerState))
		{
			int32 teamID = (GetNumPlayers() - 1) % GetPlayersPerTeam();
			UE_LOG(LogTemp, Warning, TEXT("[AUnrealTestGameMode] FILLING Player: %s Tesm: %i"), *NewPlayer->GetName(), teamID);
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

	if (gameState) { 
		gameState->SetPlayerInSession(GetNumPlayers());
		gameState->SetMaxPlayerInSession(GetMaxPlayerPerSession());
		gameState->SetMatchPhase(currentGamePhase);
	}
	
}

// Overrides default pawn class
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
// On any actor died
void AUnrealTestGameMode::ActorDied(AActor* DeadActor) {
	if (AUnrealTestCharacter* deadActor = Cast<AUnrealTestCharacter>(DeadActor)) {
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Emerald,
			FString::Printf(TEXT("[AUnrealTestGameMode][LocalRole: %s][RemoteRole: %s] Actor died %s"), *UEnum::GetValueAsString(GetLocalRole()), *UEnum::GetValueAsString(GetRemoteRole()), *DeadActor->GetName()));

			UGameInstance* gameInstance = GetWorld()->GetGameInstance();
			UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();
			EMatchPhase currentGamePhase = gameInstanceSubsystem->GetCurrentMatchPhase();

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

// Checks if player can damage another
void AUnrealTestGameMode::ProcessDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	AUnrealTestCharacter* damagedPlayer = Cast<AUnrealTestCharacter>(DamagedActor);
	AUnrealTestCharacter* damagerPlayer = Cast<AUnrealTestCharacter>(EventInstigator->GetPawn());

	if (damagedPlayer && damagerPlayer) {
		AUnrealTestPlayerState* damagedPlayerState = Cast<AUnrealTestPlayerState>(damagedPlayer->GetPlayerState());
		AUnrealTestPlayerState* damagerPlayerState = Cast<AUnrealTestPlayerState>(damagerPlayer->GetPlayerState());

		// If players have the different TeamID or their team ID is -1 (i.e.: they are in the waiting room) they can receive damage
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

// On game over event
void AUnrealTestGameMode::OnGameOverEvent(int32 TeamID)
{
	OnGameOver.Broadcast(TeamID);
}
#pragma endregion Functions