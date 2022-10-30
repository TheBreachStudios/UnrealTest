// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UnrealTest/Game/Team.h"
#include "EliminationGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AEliminationGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AEliminationGameState();

	UFUNCTION()
	void OnRep_MatchState();

	void DefaultTimer();

	virtual void PostInitializeComponents() override;
	virtual bool HasMatchStarted() const override;
	virtual bool HasMatchEnded() const override;
	virtual void HandleBeginPlay() override;
	virtual float GetPlayerStartTime(class AController* Controller) const override;
	virtual float GetPlayerRespawnDelay(class AController* Controller) const override;

	int32 ElapsedTime;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	void AutoAssignTeam(APlayerController* player);
	bool CanStartMatch();
	bool CanEndMatch();

private:
	FName GetMatchState() const { return CurrentMatchState; }
	bool IsMatchInProgress() const;
	void SetMatchState(FName newState);

protected:

	void HandleMatchIsWaitingToStart();
	void HandleMatchHasStarted();
	void HandleMatchHasEnded();
	void CreateTeams();
	void SpawnAllPlayers();
	void AssignTeamPlayerStarts();

	UPROPERTY(ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly,VisibleInstanceOnly, Category = GameState)
	FName CurrentMatchState;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName PreviousMatchState;

	FTimerHandle TimerHandle_DefaultTimer;

	TArray<Team> TeamsArray;
	TMap<int32, AActor> TeamPlayerStartsArray;
	TMap<int32, int32> TeamLivesMap;
};
