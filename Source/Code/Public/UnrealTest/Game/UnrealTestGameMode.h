// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Game Project
#include "UnrealTestGameMode.generated.h"

// Definitions
class APlayerCharacter;

// Definitions
UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
	/** No state */
	NONE,
	/** Session was created and needs players to join */
	FILLING,
	/** Session was created and all players have joined and are waiting to travel */
	WAITING,
	/** Session was created and all players have traveled to actual game level */
	PLAYING
};

UCLASS(minimalapi)
class AUnrealTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Configuration
// Configuration
protected:
	// Max player ammount per session
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxPlayerPerSession = 1;

	// Player amount per team
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PlayersPerTeam = 1;

	// Player max session search results
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxSessionSearchResults = 100;
#pragma endregion Configuration

#pragma region Variables
// Variables
protected:
	// Match phase
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMatchPhase GamePhase = EMatchPhase::NONE;
#pragma endregion // Variables


#pragma region Initialization
// Initialization

public:
	AUnrealTestGameMode();
#pragma endregion Initialization


#pragma region Getter / Setters
// Getters / Setters
public:
	// Get max player ammount per session
	UFUNCTION(BlueprintCallable)
	int32 GetMaxPlayerPerSession() { return MaxPlayerPerSession; };

	// Get player amount per team
	UFUNCTION(BlueprintCallable)
	int32 GetPlayersPerTeam() { return PlayersPerTeam; };

	// Get max session search results to find
	UFUNCTION(BlueprintCallable)
	int32 GetMaxSessionSearchResults() { return MaxSessionSearchResults; };
#pragma endregion Getter / Setters

#pragma region Overrides
// Overrides
protected:
	// On post login event;
	virtual void PostLogin(APlayerController* NewPlayer) override;
#pragma endregion // Overrides


};
