// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"

// Unreal Engine
#include "UObject/ConstructorHelpers.h"
#include "Code/Public/UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Game Project
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UnrealTestGameState.h"
#include "UnrealTest/Game/UnrealTestPlayerState.h"
#include "UnrealTest/UI/UnrealTestHUD.h"
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

		// TODO: Assumes that only exists two teams. A new implementatio will be need for more than two teams
		gameInstanceSubsystem->OnLastTeamPlayerDied.AddDynamic(this, &AUnrealTestGameMode::OnGameOverEvent);

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
			damagedPlayerState->GetTeamID() == damagerPlayerState->GetTeamID()
			&& (damagedPlayerState->GetTeamID() != -1)
			&& (damagerPlayerState->GetTeamID() != -1)
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