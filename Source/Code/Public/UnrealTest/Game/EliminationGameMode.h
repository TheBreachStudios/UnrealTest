// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealTest/Game/Team.h"
#include "EliminationGameMode.generated.h"

namespace MatchState
{
	extern ENGINE_API const FName EnteringMap;
	extern ENGINE_API const FName WaitingToStart;
	extern ENGINE_API const FName InProgress;
	extern ENGINE_API const FName WaitingPostMatch;
	extern ENGINE_API const FName LeavingMap;
	extern ENGINE_API const FName Aborted;
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

protected:
	void SetMatchState(FName newState);
	FORCEINLINE bool IsMatchInProgress() const { return CurrentMatchState == MatchState::InProgress;}

	void CreateTeams();
	void AutoAssignTeam(APlayerController* player);
	void SetupNewPlayer(APlayerController* player);

	void StartMatch();
	void EndMatch();

	bool CanStartMatch() const;
	bool CanEndMatch() const;
	
	UFUNCTION()
	void HandleWaitForPlayers();

	void HandlePlayerDeath(APlayerController* player);

	UFUNCTION()
	void HandlePlayerRespawn(APlayerController* player);

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
	virtual int32 GetNumPlayers() override;

	FORCEINLINE FName GetMatchState() const { return CurrentMatchState; }

	const float RESPAWN_DELAY = 10.f;
	const int32 MAX_TEAM_LIVES = 2;
	const int32 MAX_TEAM_SIZE = 2;
	const int32 MAX_TEAMS = 2;
	const float TIME_LIMIT = 600.f;

private:	

	const float WAIT_PLAYERS_TIME = 5.f;

	FTimerHandle TimerHandle_WaitingPlayersTimer;
	FTimerHandle TimerHandle_PlayerRespawnTimer;

	UPROPERTY(Transient)
	FName CurrentMatchState;

	int32 NumPlayers = 0;
	TArray<Team> TeamsArray;
};
