// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/EliminationGameState.h"
#include "UnrealTest/Game/EliminationGameMode.h"
#include "Net/UnrealNetwork.h"

AEliminationGameState::AEliminationGameState()
{
	CurrentMatchState = MatchState::WaitingToStart;
	PreviousMatchState = MatchState::WaitingToStart;

	CreateTeams();
}

void AEliminationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEliminationGameState, CurrentMatchState);
}

bool AEliminationGameState::IsMatchInProgress() const
{
	return GetMatchState() == MatchState::InProgress;
}

void AEliminationGameState::SetMatchState(FName newState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogGameState, Log, TEXT("Match State Changed from %s to %s"), *CurrentMatchState.ToString(), *newState.ToString());

		CurrentMatchState = newState;

		// Call the onrep to make sure the callbacks happen
		OnRep_MatchState();
	}
}

void AEliminationGameState::OnRep_MatchState()
{
	if (CurrentMatchState == MatchState::WaitingToStart)
	{
		HandleMatchIsWaitingToStart();
	}

	if (CurrentMatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (CurrentMatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}

	PreviousMatchState = CurrentMatchState;
}

void AEliminationGameState::HandleMatchIsWaitingToStart()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		GetWorldSettings()->NotifyBeginPlay();
	}
}

void AEliminationGameState::HandleMatchHasStarted()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		// Server handles this in AGameMode::HandleMatchHasStarted
		GetWorldSettings()->NotifyMatchStarted();
	}
	else
	{
		// Now that match has started, act like the base class and set replicated flag
		bReplicatedHasBegunPlay = true;
	}
}

void AEliminationGameState::HandleMatchHasEnded()
{
}

void AEliminationGameState::DefaultTimer()
{
	if (IsMatchInProgress())
	{
		ElapsedTime++;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AEliminationGameState::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation() / GetWorldSettings()->DemoPlayTimeDilation, true);
}

void AEliminationGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(TimerHandle_DefaultTimer, this, &AEliminationGameState::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation() / GetWorldSettings()->DemoPlayTimeDilation, true);
}

bool AEliminationGameState::HasMatchStarted() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		return World->bMatchStarted && GetMatchState() == MatchState::InProgress;
	}

	return false;
}

bool AEliminationGameState::HasMatchEnded() const
{
	return GetMatchState() == MatchState::WaitingPostMatch;
}

void AEliminationGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();
	SpawnAllPlayers();
}

float AEliminationGameState::GetPlayerStartTime(AController* Controller) const
{
	return ElapsedTime;
}

float AEliminationGameState::GetPlayerRespawnDelay(AController* Controller) const
{
	const AEliminationGameMode* GameMode = GetDefaultGameMode<AEliminationGameMode>();

	if (GameMode)
	{
		return GameMode->RESPAWN_DELAY;
	}

	return Super::GetPlayerRespawnDelay(Controller);
}

void AEliminationGameState::CreateTeams()
{
	const AEliminationGameMode* gameMode = GetDefaultGameMode<AEliminationGameMode>();
	if (gameMode)
	{
		for (int i =0; i< gameMode->MAX_TEAMS; i++)
		{
			Team newTeam = Team();
			TeamsArray.Add(newTeam);
			TeamLivesMap.Add(newTeam.GetTeamID(), gameMode->MAX_TEAM_LIVES);
		}		
	}	
}

void AEliminationGameState::AutoAssignTeam(APlayerController* player)
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

bool AEliminationGameState::CanStartMatch()
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
	return false;
}

bool AEliminationGameState::CanEndMatch()
{
	// Game End Requirements:
	// - One team has reached zero lives.

	for (int i = 0; i < TeamsArray.Num(); i++)
	{
		if (TeamLivesMap[TeamsArray[i].GetTeamID()] <= 0)
		{
			return true;
		}
	}
	return false;
}

//NOT IMPLEMENTED
void AEliminationGameState::SpawnAllPlayers()
{
	AEliminationGameMode* gameMode = Cast<AEliminationGameMode>(AuthorityGameMode);
	if (gameMode)
	{
		for (int i = 0; i < TeamsArray.Num(); i++)
		{
			TArray<APlayerController*> teammates = TeamsArray[i].GetTeammates();
			for (int k = 0; k < teammates.Num(); k++)
			{
				// TODO: Assign a team appropriate player start
				gameMode->RestartPlayer(teammates[k]);
			}
		}
	}	
}

void AEliminationGameState::AssignTeamPlayerStarts()
{
}
