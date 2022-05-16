// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UnrealTestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region Variables
// Variables
private:
	// Team identifier
	UPROPERTY(Replicated)
	int32 TeamID = -1;
#pragma endregion Variables

#pragma region Getters / Setters
// Getters / Setters
public:
	// Get team identifier
	UFUNCTION(BlueprintCallable, Category = "TeamHandling")
	int32 GetTeamID() { return TeamID; }

	// Set team identifier
	UFUNCTION(BlueprintCallable, Category = "TeamHandling")
	void SetTeamID(int32 NewTeamID) { TeamID = NewTeamID; }
#pragma endregion Getters / Setters

#pragma region Overrides
// Overrides
	void CopyProperties(APlayerState* PlayerState) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion Overrides
};
