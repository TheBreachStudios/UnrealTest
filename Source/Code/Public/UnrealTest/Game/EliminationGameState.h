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

	DECLARE_MULTICAST_DELEGATE_TwoParams(FEliminationStateDelegate, int32, int32);

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
	void RegisterTeams(TArray<Team*> teams);

	int32 GetTeamID(APlayerController* player) const;
	void GetAllTeamLives(APlayerController* player, int32 &ownTeamLives, int32 &enemyTeamLives);

	FORCEINLINE int32 GetMaxLives() const { return MAX_TEAM_LIVES; }

	UFUNCTION()
	void OnRep_MatchState();

	FEliminationStateDelegate OnTeamLivesChanged;
	

	void LockPlayerInput(APlayerController* player);
	void UnlockPlayerInput(APlayerController* player);

private:
	bool IsMatchInProgress() const;
	void SetMatchState(FName newState);

protected:

	void HandleMatchIsWaitingForPlayers();
	void HandleMatchIsWaitingToStart();
	void HandleMatchHasStarted();
	void HandleMatchHasEnded();
	void HandlePlayerDeath(APlayerController* player);

	void ResetTeamLives();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName CurrentMatchState;

	UPROPERTY(Transient, BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName PreviousMatchState;

	TArray<APlayerController*> PlayersArray;
	TArray<Team*> TeamsPtrArray;
	TMap<Team*, int32> TeamLivesMap;

	const int32 MAX_TEAM_LIVES = 10;
};
