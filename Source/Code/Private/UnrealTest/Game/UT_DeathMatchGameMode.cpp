// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/UT_DeathMatchGameMode.h"

//Unreal includes
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameStateBase.h"

//ProjectIncludes 
#include "UnrealTest/Character/UT_PlayerState.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UT_DeathMatchGameState.h"
#include "UnrealTest/Components/UT_CustomPlayerStart.h"

AUT_DeathMatchGameMode::AUT_DeathMatchGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultPawnClass = AUnrealTestCharacter::StaticClass();
	PlayerStateClass = AUT_PlayerState::StaticClass();
	GameStateClass = AUT_DeathMatchGameState::StaticClass();

	NumTeams = 2;
	NumberOfKillsNeededToEndMatch = 10;
	
}

AActor* AUT_DeathMatchGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> posibleSpawns;
	APlayerStart* bestStart = nullptr;

	//TODO Cambiar
	AUT_PlayerState* NewPlayerState = Cast<AUT_PlayerState>(Player->PlayerState);
	const int32 TeamNum = ChooseTeam(NewPlayerState);
	NewPlayerState->SetTeamNum(TeamNum);

	//Get All SpawnPoints from map
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* start = *It;
		if (start->IsA<AUT_CustomPlayerStart>())
		{
			//Check team restrictions
			if (CheckStartTeam(start, Player))
			{
				posibleSpawns.Add(start);
			}
		}
	}

	//Choose Random StartPos
	if (!bestStart)
	{
		if (posibleSpawns.Num() > 0)
		{
			bestStart = posibleSpawns[FMath::RandHelper(posibleSpawns.Num())];
		}
	}
	//In case no best start start by default
	return bestStart ? bestStart : Super::ChoosePlayerStart_Implementation(Player);
}

bool AUT_DeathMatchGameMode::CheckStartTeam(APlayerStart* PlayerStart, AController* Player) const
{
	if (Player)
	{
		AUT_CustomPlayerStart* customStart = Cast<AUT_CustomPlayerStart>(PlayerStart);
		AUT_PlayerState* playerState = Cast<AUT_PlayerState>(Player->PlayerState);

		if (playerState && customStart && customStart->SpawnTeam == playerState->GetTeamNum())
		{
			return true;
		}
	}
	return false;
}

void AUT_DeathMatchGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NumPlayers == PlayerNumberToStartGame)
	{
		OnMatchStart.Broadcast();
		StartMatch();
	}
}

void AUT_DeathMatchGameMode::Killed(AController* KilledPlayer)
{
	AUT_PlayerState* killedPlayerState = KilledPlayer->PlayerState ? Cast<AUT_PlayerState>(KilledPlayer->PlayerState) : nullptr;

	if (killedPlayerState)
	{
		killedPlayerState->AddDeath(killedPlayerState->TeamNumber);
		HandleKill(killedPlayerState->GetTeamNum());
	}
}

void AUT_DeathMatchGameMode::HandleKill(int32 Team)
{
	AUT_DeathMatchGameState* const gameState = GetWorld()->GetGameState<AUT_DeathMatchGameState>();
	if (gameState)
	{
		for (size_t i = 0; i < gameState->TeamScores.Num(); i++)
		{
			if (gameState->TeamScores[i] == NumberOfKillsNeededToEndMatch)
			{
				OnMatchEnd.Broadcast();

				EndMatch();
			}
		}
	}
}

int32 AUT_DeathMatchGameMode::ChooseTeam(AUT_PlayerState* PlayerState) const
{
	int32 playersTeam0 = 0;
	int32 playersTeam1 = 0;

	// Array of all Player States
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AUT_PlayerState* state = Cast<AUT_PlayerState>(GameState->PlayerArray[i]);
		if (state)
		{
			//Add one in case 
			state->GetTeamNum() == 0 ? playersTeam0++ : playersTeam1++;
		}
	}
	//return the index of the team least populated
	return playersTeam0 <= playersTeam1 ? 0 : 1;
}

