// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

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
class AUnrealTestGameMode : public AGameMode
{
	GENERATED_BODY()

#pragma region Delegates
// Delegates
public:
	// On session creation completed delegate
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, int32, DefeatedTeamID);
	UPROPERTY(BlueprintAssignable, Category = "SessionHandling")
	FOnGameOver OnGameOver;
#pragma endregion Delegates

#pragma region Configuration
// Configuration
protected:
	// Max player ammount per session
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameMode")
	int32 MaxPlayerPerSession = 1;

	// Player amount per team
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 PlayersPerTeam = 1;

	// Player max session search results
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SessionHandling")
	int32 MaxSessionSearchResults = 100;

	// Champions data table
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Champions")
	class UDataTable* ChampionsDataTable;
#pragma endregion Configuration

#pragma region Variables
// Variables
protected:
	// Match phase
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	EMatchPhase GamePhase = EMatchPhase::NONE;
#pragma endregion Variables


#pragma region Initialization
// Initialization

public:
	// Constructor
	AUnrealTestGameMode(const FObjectInitializer& ObjectInitializer);
#pragma endregion Initialization


#pragma region Getter / Setters
// Getters / Setters
public:
	// Get max player ammount per session
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetMaxPlayerPerSession() { return MaxPlayerPerSession; };

	// Get player amount per team
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetPlayersPerTeam() { return PlayersPerTeam; };

	// Get max session search results to find
	UFUNCTION(BlueprintCallable, Category = "SessionHandling")
	int32 GetMaxSessionSearchResults() { return MaxSessionSearchResults; };
#pragma endregion Getter / Setters

#pragma region Overrides
// Overrides
protected:
	// On pre login event
	virtual void PreLogin( const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMe) override;
	
	// On login event
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	// On post login event
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	// Overrides default pawn class
	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

#pragma endregion Overrides

#pragma region Functions
public:
	// On any actor died
	void ActorDied(AActor* DeadActor);

	// Checks if player can damage another
	void ProcessDamage(AActor* DamagedAtor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass);

	// On game over event
	UFUNCTION()
	void OnGameOverEvent(int32 TeamID);
#pragma endregion Functions
};
