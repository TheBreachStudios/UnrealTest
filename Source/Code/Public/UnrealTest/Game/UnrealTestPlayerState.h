// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

// Game Project
#include "UnrealTestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region Delegates
// Delegates
public:
	// On player team ID changed.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTeamIDChanged, int32, TeamID);
	UPROPERTY(BlueprintAssignable, Category = "SessionHandling")
	FOnPlayerTeamIDChanged OnPlayerTeamIDChanged;

#pragma endregion Delegates

#pragma region Initialization
// Initialization
public:
	// Setup replicated properties.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma region Initialization

#pragma region Variables
// Variables
private:
	// Team identifier.
	UPROPERTY(Replicated)
	int32 TeamID = -1;
#pragma endregion Variables

#pragma region Getters / Setters
// Getters / Setters
public:
	// Get team identifier.
	UFUNCTION(BlueprintCallable, Category = "TeamHandling")
	FORCEINLINE int32 GetTeamID() const { return TeamID; }

	// Set team identifier.
	UFUNCTION(BlueprintCallable, Category = "TeamHandling")
	void SetTeamID(int32 NewTeamID) { TeamID = NewTeamID; }
#pragma endregion Getters / Setters
};
