// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UnrealNetwork.h"
#include "UnrealTest/Game/EliminationGameState.h"
#include "UnrealTest/Game/EliminationGameMode.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/ChampionPlayerController.h"

AEliminationGameState::AEliminationGameState()
{
	CurrentMatchState = MatchState::EnteringMap;
	PreviousMatchState = MatchState::EnteringMap;
}

void AEliminationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEliminationGameState, CurrentMatchState);
}

bool AEliminationGameState::IsMatchInProgress() const
{
	return CurrentMatchState == MatchState::InProgress;
}

void AEliminationGameState::SetMatchState(FName newState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogGameState, Log, TEXT("Match State Changed from %s to %s"), *CurrentMatchState.ToString(), *newState.ToString());

		CurrentMatchState = newState;

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

void AEliminationGameState::HandleMatchIsWaitingForPlayers()
{
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

void AEliminationGameState::HandlePlayerDeath(APlayerController* player)
{
	for (int i = 0; i < TeamsPtrArray.Num(); i++)
	{
		if (TeamsPtrArray[i]->IsPlayerInTeam(player))
		{
			TeamLivesMap[TeamsPtrArray[i]->GetTeamID()]--;
		}
	}
}

void AEliminationGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

bool AEliminationGameState::HasMatchStarted() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		return World->bMatchStarted && CurrentMatchState == MatchState::InProgress;
	}

	return false;
}

bool AEliminationGameState::HasMatchEnded() const
{
	return CurrentMatchState == MatchState::WaitingPostMatch;
}

void AEliminationGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();

}

bool AEliminationGameState::CanStartMatch() const
{
	// Game Start Requirements:
	// - At least 1 player in each team.
	// - All players set they are ready.

	for (int i = 0; i < TeamsPtrArray.Num(); i++)
	{
		if (TeamsPtrArray[i]->GetNumTeammates() <= 0)
		{
			return false;
		}

		if (!TeamsPtrArray[i]->AllTeammatesReady())
		{
			return false;
		}
	}
	return true;
}

bool AEliminationGameState::CanEndMatch() const
{
	// Game End Requirements:
	// - One team has reached zero lives.

   for (int i = 0; i < TeamsPtrArray.Num(); i++)
   {
		if (TeamLivesMap[TeamsPtrArray[i]->GetTeamID()] <= 0)
		{
			return true;
		}
   }
	return false;
}

void AEliminationGameState::RegisterPlayer(APlayerController* player)
{
	if (!PlayersArray.Contains(player))
	{
		PlayersArray.Add(player);

		AChampionPlayerController* championController = Cast<AChampionPlayerController>(player);
		if (championController != nullptr)
		{
			championController->OnPlayerDeathEvent.AddUObject(this, &AEliminationGameState::HandlePlayerDeath);
		}
	}
}

void AEliminationGameState::RegisterTeams(TArray<Team*> teams, int32 teamLives)
{
	TeamsPtrArray = teams;
	for (int i = 0; i < teams.Num(); i++)
	{
		TeamLivesMap.Add(teams[i]->GetTeamID(), teamLives);
	}
}
