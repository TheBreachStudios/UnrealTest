// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/EliminationGameState.h"
#include "UnrealTest/Game/EliminationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UnrealTest/Character/ChampionCharacter.h"

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

//void AEliminationGameState::OnRep_TeamLivesMap()
//{
//}

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

void AEliminationGameState::HandlePlayerDeath()
{
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

float AEliminationGameState::GetPlayerRespawnDelay(AController* Controller) const
{
	const AEliminationGameMode* GameMode = GetDefaultGameMode<AEliminationGameMode>();

	if (GameMode)
	{
		return GameMode->RESPAWN_DELAY;
	}

	return Super::GetPlayerRespawnDelay(Controller);
}

bool AEliminationGameState::CanStartMatch() const
{
	// Game Start Requirements:
	// - At least 1 player in each team.
	// - All players set they are ready.

	//for (int i = 0; i < TeamsPtrArray.Num(); i++)
	//{
	//	if (TeamsPtrArray[i]->GetNumTeammates() <= 0)
	//	{
	//		return false;
	//	}

	//	if (!TeamsPtrArray[i]->AllTeammatesReady())
	//	{
	//		return false;
	//	}
	//}
	return false;
}

bool AEliminationGameState::CanEndMatch() const
{
	 // Game End Requirements:
	 // - One team has reached zero lives.

	//for (int i = 0; i < TeamsPtrArray.Num(); i++)
	//{
	//	if (TeamLivesMap[TeamsPtrArray[i]->GetTeamID()] <= 0)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

void AEliminationGameState::RegisterPlayer(APlayerController* player)
{
	if (!PlayersArray.Contains(player))
	{
		PlayersArray.Add(player);
	}
}

void AEliminationGameState::LockPlayers()
{
}

void AEliminationGameState::UnlockPlayers()
{
}
