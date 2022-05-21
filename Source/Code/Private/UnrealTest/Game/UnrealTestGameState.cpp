// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/UnrealTestGameState.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// GameProject
#include "Code/Public/UnrealTest/Game/UnrealTestGameMode.h"

#pragma region Initialization
// Initialization

// Constructor
AUnrealTestGameState::AUnrealTestGameState() :
	MatchPhase(EMatchPhase::NONE)
{
	bReplicates = true;
}
#pragma endregion Initialization

#pragma region Getters / Setters
// Set current player count in session
void AUnrealTestGameState::SetPlayerInSession(int32 NewPlayerCount) {
	CurrentPlayersInSession = NewPlayerCount;
	OnPlayerJoined.Broadcast(CurrentPlayersInSession);
}

// Set max player count in session
void AUnrealTestGameState::SetMaxPlayerInSession(int32 NewMaxPlayerCount) {
	MaxPlayersInSession = NewMaxPlayerCount;
	OnPlayerMaxChanged.Broadcast(NewMaxPlayerCount);
};

// Set current match phase
void AUnrealTestGameState::SetMatchPhase(EMatchPhase NewPhase) {
	MatchPhase = NewPhase;
	OnMatchPhaseChanged.Broadcast(NewPhase);
}
#pragma endregion Getters / Setters

#pragma region Override
// Override

	// Replication settings
void AUnrealTestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnrealTestGameState, CurrentPlayersInSession);
	DOREPLIFETIME(AUnrealTestGameState, MaxPlayersInSession);
	DOREPLIFETIME(AUnrealTestGameState, MatchPhase);
}
#pragma endregion Override
