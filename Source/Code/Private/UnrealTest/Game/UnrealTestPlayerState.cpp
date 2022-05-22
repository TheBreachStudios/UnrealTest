// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/UnrealTestPlayerState.h"
#include "Net/UnrealNetwork.h"


#pragma region Initialization
// Setup replicated properties.
void AUnrealTestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AUnrealTestPlayerState, TeamID);
}
#pragma endregion Initialization