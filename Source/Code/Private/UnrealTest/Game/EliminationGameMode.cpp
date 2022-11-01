// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/EliminationGameMode.h"
#include "UnrealTest/Game/EliminationGameState.h"
#include "UnrealTest/UI/PlayerHUD.h"
#include "UnrealTest/Character/ChampionPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"


AEliminationGameMode::AEliminationGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_VanguardChampion"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	HUDClass = APlayerHUD::StaticClass();;
	GameStateClass = AEliminationGameState::StaticClass();
	PlayerControllerClass = AChampionPlayerController::StaticClass();
	//PlayerStateClass = AEliminationPlayerState::StaticClass();
	CurrentMatchState = MatchState::EnteringMap;
	NumPlayers = 0;

	CreateTeams();
}

void AEliminationGameMode::StartMatch()
{
	if (HasMatchStarted())
	{
		return;
	}

	SetMatchState(MatchState::InProgress);
	// TODO: Unlock the players inputs.
}

void AEliminationGameMode::EndMatch()
{
	if (!IsMatchInProgress())
	{
		return;
	}

	SetMatchState(MatchState::WaitingPostMatch);
	// TODO: Lock the players inputs.
}

bool AEliminationGameMode::CanStartMatch() const
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

bool AEliminationGameMode::CanEndMatch() const
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

void AEliminationGameMode::GatherPlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* Start = *It;
		AllPlayerStartsArray.Add(Start);
	}
}

void AEliminationGameMode::RegisterTeams()
{
	if (GameState != nullptr)
	{
		AEliminationGameState* eliminationState = Cast<AEliminationGameState>(GameState);
		if (eliminationState != nullptr)
		{
			TArray<Team*> tempArray;
			for (int i = 0; i < TeamsArray.Num(); i++)
			{
				tempArray.Add(&TeamsArray[i]);
			}
			eliminationState->RegisterTeams(tempArray, MAX_TEAM_LIVES);
		}
	}
}

void AEliminationGameMode::SetMatchState(FName newState)
{
	if (CurrentMatchState == newState) { return; }

	UE_LOG(LogGameMode, Display, TEXT("Match State: %s -> %s"), *CurrentMatchState.ToString(), *newState.ToString());

	CurrentMatchState = newState;
}

void AEliminationGameMode::CreateTeams()
{
	for (int i = 0; i < MAX_TEAMS; i++)
	{
		Team newTeam = Team();
		TeamsArray.Add(newTeam);
	}
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
	SetMatchState(MatchState::EnteringMap);
	GatherPlayerStarts();

	GetWorldTimerManager().SetTimer(TimerHandle_WaitingPlayersTimer, this, &AEliminationGameMode::HandleWaitForPlayers, WAIT_PLAYERS_TIME);
}

void AEliminationGameMode::StartPlay()
{
	Super::StartPlay();

	RegisterTeams();

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
			SetupNewPlayer(NewPlayer);
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

void AEliminationGameMode::HandleWaitForPlayers()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_WaitingPlayersTimer);
	SetMatchState(MatchState::WaitingToStart);
}

void AEliminationGameMode::HandlePlayerDeath(APlayerController* player)
{
	FTimerHandle timerHandle;
	FTimerDelegate timerDel;
	timerDel.BindUFunction(this, FName("HandlePlayerRespawn"), player);
	GetWorldTimerManager().SetTimer(timerHandle, timerDel, RESPAWN_DELAY, false);
}

void AEliminationGameMode::HandlePlayerRespawn(APlayerController* player)
{
	int32 randomIdx = FMath::RandRange(0, AllPlayerStartsArray.Num());
	//APlayerStart* playerStart = AllPlayerStartsArray[randomIdx];
	//FVector respawnLocation = playerStart->GetActorLocation();	
	//FRotator respawnRotation = playerStart->GetActorRotation();
	//RestartPlayerAtTransform(player, playerStart->GetActorTransform());
	RestartPlayer(player);
}

void AEliminationGameMode::RemovePlayerControllerFromPlayerCount(APlayerController* PC)
{
	if (PC == nullptr) { return; }

	if (PC->HasClientLoadedCurrentWorld())
	{
		NumPlayers--;
	}
}

void AEliminationGameMode::AutoAssignTeam(APlayerController* player)
{
	// Check if the teams were already created.
	if (TeamsArray.Num() > 0)
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
					//TEMP: Auto ready for now
					TeamsArray[i].SetTeammateReadiness(player, true);
					wasAssigned = true;
					break;
				}
			}
			minTeammates++;
		} while (!wasAssigned);
	}
}

void AEliminationGameMode::SetupNewPlayer(APlayerController* player)
{
	AEliminationGameState* eliminationGameState = Cast<AEliminationGameState>(GameState);
	if (eliminationGameState != nullptr)
	{
		AutoAssignTeam(player);
		eliminationGameState->RegisterPlayer(player);
		AChampionPlayerController* playerController = Cast<AChampionPlayerController>(player);
		if (playerController != nullptr)
		{
			playerController->OnPlayerDeathEvent.AddUObject(this, &AEliminationGameMode::HandlePlayerDeath);
		}
	}
}
