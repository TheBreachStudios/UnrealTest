// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/EliminationGameMode.h"
#include "UnrealTest/Game/EliminationGameState.h"
#include "UnrealTest/Game/EliminationPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "UnrealTest/UI/PlayerHUD.h"

AEliminationGameMode::AEliminationGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Champions/Blueprints/BP_VanguardChampion"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	HUDClass = APlayerHUD::StaticClass();;
	//GameStateClass = AEliminationGameState::StaticClass();
	//PlayerStateClass = AEliminationPlayerState::StaticClass();
	CurrentMatchState = MatchState::WaitingToStart;
	NumPlayers = 0;

	CreateTeams();
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
	// Game Start Requirements:
	// - At least 1 player in each team.
	// - All players set they are ready.

	for (int i = 0; i < TeamsArray.Num(); i++)
	{
		if (TeamsArray[i].GetNumTeammates() <= 0)
		{
			return false;
		}

		if (!TeamsArray[i].AllTeammatesReady())
		{
			return false;
		}
	}
	return true;
}

bool AEliminationGameMode::CanEndMatch()
{
	// Game End Requirements:
	// - One team has reached zero lives

	for (int i = 0; i < TeamsArray.Num(); i++)
	{
		if (TeamsArray[i].GetTeamLives() <= 0)
		{
			return true;
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
	return GameState && GameState->HasMatchStarted() && GetMatchState() == MatchState::InProgress;
}

bool AEliminationGameMode::HasMatchEnded() const
{
	return GameState && GameState->HasMatchEnded() && GetMatchState() == MatchState::WaitingPostMatch;
}

void AEliminationGameMode::PostLogin(APlayerController* NewPlayer)
{
	if (NewPlayer->HasClientLoadedCurrentWorld())
	{
		NumPlayers++;
		AutoAssignTeam(NewPlayer);
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

void AEliminationGameMode::CreateTeams()
{
	Team newTeam1 = Team(MAX_TEAM_LIVES);
	Team newTeam2 = Team(MAX_TEAM_LIVES);

	TeamsArray.Add(newTeam1);
	TeamsArray.Add(newTeam2);
}

void AEliminationGameMode::AutoAssignTeam(APlayerController* player)
{
	bool wasAssigned = false;
	int minTeammates = 0;
	do
	{
		for (int i = 0; i < TeamsArray.Num(); i++)
		{
			if (TeamsArray[i].GetNumTeammates() <= minTeammates)
			{
				TeamsArray[i].AssignTeammate(player);
				//TEMP Auto ready
				TeamsArray[i].SetTeammateReadiness(player, true);
				wasAssigned = true;
				break;
			}
		}
		minTeammates++;
	} while (!wasAssigned);
}

void AEliminationGameMode::SpawnAllPlayers()
{
	for (int i = 0; i < TeamsArray.Num(); i++)
	{
		TArray<APlayerController*> teammates = TeamsArray[i].GetTeammates();
		for (int k = 0; k < teammates.Num(); k++)
		{
			// TODO: Assign a team appropriate player start
			RestartPlayer(teammates[k]);
		}
	}
}

void AEliminationGameMode::RemovePlayerControllerFromPlayerCount(APlayerController* PC)
{
	if (PC == nullptr) { return; }

	if (PC->HasClientLoadedCurrentWorld())
	{
		NumPlayers--;
	}
}
