// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

// Game Project
#include "UnrealTestGameState.generated.h"

// Definitions
class AUnrealTestGameMode;
enum class EMatchPhase : uint8;

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestGameState : public AGameStateBase
{
	GENERATED_BODY()

#pragma region Variables
// Variables
private:
	// Game mode reference
	AUnrealTestGameMode* GameMode;

	// Current players in session
	UPROPERTY(Replicated)
	int32 CurrentPlayersInSession = 0;

	// Max players in session
	UPROPERTY(Replicated)
	int32 MaxPlayersInSession = 0;

	// Match phase
	UPROPERTY(Replicated)
	EMatchPhase GamePhase;

#pragma endregion Variables

#pragma region Initialization
	// Override

	// Constructor
	AUnrealTestGameState();
#pragma endregion Initialization

#pragma region Override
// Override

	// Replication settings
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion Override

#pragma region Getters / Setters
// Getters / Setters

public:
	// Get current player count in session
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentPlayersInSession() { return CurrentPlayersInSession; }

	// Get max player count in session
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetMaxPlayerInSession() { return MaxPlayersInSession; }

	// Get current match phase
	UFUNCTION(BlueprintPure, BlueprintCallable)
	EMatchPhase GetMatchPhase() { return GamePhase; }
	
	// Set current player count in session
	UFUNCTION(BlueprintCallable)
	void SetPlayerInSession(int32 NewPlayerCount) { CurrentPlayersInSession = NewPlayerCount; }

	// Set max player count in session
	UFUNCTION(BlueprintCallable)
	void SetMaxPlayerInSession(int32 NewMaxPlayerCount) { MaxPlayersInSession = NewMaxPlayerCount; };

	// Set current match phase
	UFUNCTION(BlueprintCallable)
	void SetMatchPhase(EMatchPhase NewPhase) { GamePhase = NewPhase; }

#pragma endregion Getters / Setters

};
