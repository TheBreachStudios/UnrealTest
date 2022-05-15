// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/UnrealTestGameState.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// GameProject
#include "Code/Public/UnrealTest/Game/UnrealTestGameMode.h"

#pragma region Initialization
// Initialization
AUnrealTestGameState::AUnrealTestGameState()
{
	bReplicates = true;
}
#pragma endregion Initialization

#pragma region Override
// Override
void AUnrealTestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnrealTestGameState, CurrentPlayersInSession);
	DOREPLIFETIME(AUnrealTestGameState, MaxPlayersInSession);
}
#pragma endregion Override