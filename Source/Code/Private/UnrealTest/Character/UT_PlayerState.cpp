// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/UT_PlayerState.h"

//Unreal Includes
#include "Net/UnrealNetwork.h"	
#include "GameFramework/PlayerController.h"

//Porject Includes 
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UT_DeathMatchGameState.h"

AUT_PlayerState::AUT_PlayerState()
{
	TeamNumber = 0;
}

void AUT_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUT_PlayerState, TeamNumber);
}

void AUT_PlayerState::OnRep_TeamNumberChanged()
{
	//In case team has changed update colores
	UpdateTeamColors();
}

void AUT_PlayerState::AddDeath(const int32 Team)
{
	//Change team because increment deaths on the other team
	int32 otherTeam = Team == 0 ? 1 : 0;

	AUT_DeathMatchGameState* const gameState = GetWorld()->GetGameState<AUT_DeathMatchGameState>();
	if (gameState)
	{
		gameState->TeamScores[otherTeam]++;	
	}
}

void AUT_PlayerState::SetTeamNum(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
	UpdateTeamColors();
}

int32 AUT_PlayerState::GetTeamNum() const
{
	return TeamNumber;
}

void AUT_PlayerState::UpdateTeamColors()
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (AUnrealTestCharacter* playerChar = Cast<AUnrealTestCharacter>(OwnerController->GetCharacter()))
		{
			playerChar->UpdateTeamColors(TeamNumber);
		}
	}
}

void AUT_PlayerState::ClientInitialize(AController* InController)
{
	Super::ClientInitialize(InController);

	UpdateTeamColors();
}
