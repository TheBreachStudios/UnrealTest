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

	virtual void PostInitializeComponents() override;
	virtual bool HasMatchStarted() const override;
	virtual bool HasMatchEnded() const override;
	virtual void HandleBeginPlay() override;
	virtual float GetPlayerRespawnDelay(class AController* Controller) const override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	bool CanStartMatch() const;
	bool CanEndMatch() const;

	void RegisterPlayer(APlayerController* player);
	void LockPlayers();
	void UnlockPlayers();

	UFUNCTION()
	void OnRep_MatchState();
	//UFUNCTION()
	//void OnRep_TeamLivesMap();

private:
	bool IsMatchInProgress() const;
	void SetMatchState(FName newState);

protected:

	void HandleMatchIsWaitingForPlayers();
	void HandleMatchIsWaitingToStart();
	void HandleMatchHasStarted();
	void HandleMatchHasEnded();
	void HandlePlayerDeath();

	UPROPERTY(ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName CurrentMatchState;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName PreviousMatchState;

	TArray<APlayerController*> PlayersArray;

	TMap<int32, int32> TeamLivesMap;
};
