// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealTest/Game/Team.h"
#include "EliminationGameMode.generated.h"

namespace MatchState
{
	//extern ENGINE_API const FName EnteringMap;			// We are entering this map, actors are not yet ticking
	extern ENGINE_API const FName WaitingToStart;		// Actors are ticking, but the match has not yet started
	extern ENGINE_API const FName InProgress;			// Normal gameplay is occurring. Specific games will have their own state machine inside this state
	extern ENGINE_API const FName WaitingPostMatch;		// Match has ended so we aren't accepting new players, but actors are still ticking
	//extern ENGINE_API const FName LeavingMap;			// We are transitioning out of the map to another location
}

/**
 * 
 */
UCLASS()
class UNREALTEST_API AEliminationGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEliminationGameMode();

	FORCEINLINE FName GetMatchState() const { return CurrentMatchState; }

	void StartMatch();
	void EndMatch();
	bool CanStartMatch();
	bool CanEndMatch();

protected:
	void SetMatchState(FName newState);
	FORCEINLINE bool IsMatchInProgress() const { return CurrentMatchState == MatchState::InProgress;}

	void SpawnAllPlayers();

public:

	/** Removes the passed in player controller from the correct count for player/spectator/tranistioning **/
	void RemovePlayerControllerFromPlayerCount(APlayerController* PC);

	virtual void Tick(float DeltaSeconds) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual bool HasMatchStarted() const override;
	virtual bool HasMatchEnded() const override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual int32 GetNumPlayers() override;//

	const float RESPAWN_DELAY = 5.f;
	const int32 MAX_TEAM_LIVES = 10;
	const int32 MAX_TEAM_SIZE = 2;
	const int32 MAX_TEAMS = 2;
	const float TIME_LIMIT = 600.f;

private:

	UPROPERTY(Transient)
	FName CurrentMatchState;

	TArray<Team> TeamsArray;
	int32 NumPlayers = 0;
};
