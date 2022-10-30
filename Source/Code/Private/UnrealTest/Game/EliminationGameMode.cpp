// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/EliminationGameMode.h"
#include "UnrealTest/Game/EliminationGameState.h"
#include "UnrealTest/Game/EliminationPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "UnrealTest/UI/PlayerHUD.h"

AEliminationGameMode::AEliminationGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Champions/Blueprints/BP_VanguardChampion"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	HUDClass = APlayerHUD::StaticClass();;
	GameStateClass = AEliminationGameState::StaticClass();
	//PlayerStateClass = AEliminationPlayerState::StaticClass();
	CurrentMatchState = MatchState::WaitingToStart;
	NumPlayers = 0;
}

void AEliminationGameMode::StartMatch()
{
	if (HasMatchStarted())
	{
		return;
	}

	SpawnAllPlayers();

	SetMatchState(MatchState::InProgress);
}

void AEliminationGameMode::EndMatch()
{
	if (!IsMatchInProgress())
	{
		return;
	}

	SetMatchState(MatchState::WaitingPostMatch);
}

bool AEliminationGameMode::CanStartMatch()
{
	if (GameState != nullptr)
	{
		AEliminationGameState* eliminationGameState = Cast<AEliminationGameState>(GameState);
		if (eliminationGameState != nullptr)
		{
			return eliminationGameState->CanStartMatch();
		}
	}
	return false;
}

bool AEliminationGameMode::CanEndMatch()
{
	if (GameState != nullptr)
	{
		AEliminationGameState* eliminationGameState = Cast<AEliminationGameState>(GameState);
		if (eliminationGameState != nullptr)
		{
			return eliminationGameState->CanEndMatch();
		}
	}
	return false;
}

void AEliminationGameMode::SetMatchState(FName newState)
{
	if (CurrentMatchState == newState) { return; }

	UE_LOG(LogGameMode, Display, TEXT("Match State: %s -> %s"), *CurrentMatchState.ToString(), *newState.ToString());

	CurrentMatchState = newState;
}

void AEliminationGameMode::SpawnAllPlayers()
{
}

void AEliminationGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (CanStartMatch())
		{
			UE_LOG(LogGameMode, Log, TEXT("EliminationGameMode CanStartMatch"));
			StartMatch();
		}
	}
	if (GetMatchState() == MatchState::InProgress)
	{
		if (CanEndMatch())
		{
			UE_LOG(LogGameMode, Log, TEXT("EliminationGameMode CanEndMatch"));
			EndMatch();
		}
	}
}

void AEliminationGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	SetMatchState(MatchState::WaitingToStart);
}

void AEliminationGameMode::StartPlay()
{
	Super::StartPlay();

	if (GetMatchState() == MatchState::WaitingToStart && CanStartMatch())
	{
		StartMatch();
	}
}

bool AEliminationGameMode::HasMatchStarted() const
{
	return GameState != nullptr && GameState->HasMatchStarted() && GetMatchState() == MatchState::InProgress;
}

bool AEliminationGameMode::HasMatchEnded() const
{
	return GameState != nullptr && GameState->HasMatchEnded() && GetMatchState() == MatchState::WaitingPostMatch;
}

void AEliminationGameMode::PostLogin(APlayerController* NewPlayer)
{
	if (NewPlayer->HasClientLoadedCurrentWorld())
	{
		NumPlayers++;
		if (GameState != nullptr)
		{
			AEliminationGameState* eliminationGameState = Cast<AEliminationGameState>(GameState);
			if (eliminationGameState != nullptr)
			{
				eliminationGameState->AutoAssignTeam(NewPlayer);
			}
		}
	}

	Super::PostLogin(NewPlayer);
}

void AEliminationGameMode::Logout(AController* Exiting)
{
	APlayerController* PC = Cast<APlayerController>(Exiting);
	if (PC != nullptr)
	{
		RemovePlayerControllerFromPlayerCount(PC);
	}

	Super::Logout(Exiting);
}

int32 AEliminationGameMode::GetNumPlayers()
{
	return NumPlayers;
}

void AEliminationGameMode::RemovePlayerControllerFromPlayerCount(APlayerController* PC)
{
	if (PC == nullptr) { return; }

	if (PC->HasClientLoadedCurrentWorld())
	{
		NumPlayers--;
	}
}
