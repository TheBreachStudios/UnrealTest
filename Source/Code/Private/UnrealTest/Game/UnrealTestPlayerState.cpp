// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/UnrealTestPlayerState.h"
#include "Net/UnrealNetwork.h"


#pragma region Overrides
void AUnrealTestPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    AUnrealTestPlayerState* playerState = Cast<AUnrealTestPlayerState>(PlayerState);
    if (ensure(playerState))
    {
        TeamID = playerState->TeamID;
        OnPlayerTeamIDChanged.Broadcast(TeamID);
    }
}


void AUnrealTestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AUnrealTestPlayerState, TeamID);
}

#pragma endregion Overrides