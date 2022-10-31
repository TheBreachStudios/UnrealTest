// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UnrealTest/Game/Team.h"
#include "UnrealTest/Character/ChampionPlayerController.h"
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

	virtual void PostInitializeComponents() override;
	virtual bool HasMatchStarted() const override;
	virtual bool HasMatchEnded() const override;
	virtual void HandleBeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	bool CanStartMatch() const;
	bool CanEndMatch() const;

	void RegisterPlayer(APlayerController* player);
	void RegisterTeams(TArray<Team*> teams, int32 teamLives);

	UFUNCTION()
	void OnRep_MatchState();

private:
	bool IsMatchInProgress() const;
	void SetMatchState(FName newState);

protected:

	void HandleMatchIsWaitingForPlayers();
	void HandleMatchIsWaitingToStart();
	void HandleMatchHasStarted();
	void HandleMatchHasEnded();
	void HandlePlayerDeath(APlayerController* player);

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName CurrentMatchState;

	UPROPERTY(Transient, BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName PreviousMatchState;

	TArray<APlayerController*> PlayersArray;
	TArray<Team*> TeamsPtrArray;
	TMap<int32, int32> TeamLivesMap;
};
