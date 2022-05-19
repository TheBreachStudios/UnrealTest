// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/UI/MasterWidget.h"
#include "PlayerHUDWidget.generated.h"

// Definitions
class URichTextBlock;
class UHealthBarWidget;
class AUnrealTestGameState;
enum class EMatchPhase : uint8;

/**
 * 
 */
UCLASS()
class UNREALTEST_API UPlayerHUDWidget : public UMasterWidget
{
	GENERATED_BODY()

#pragma region Variables
// Variables
private:
	// Game state reference
	AUnrealTestGameState* GameState = nullptr;

	// Current game phase
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MatchInfo", meta = (AllowPrivateAccess = "true"))
	EMatchPhase CurrentMatchPhase;

	// Current players count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MatchInfo", meta = (AllowPrivateAccess = "true"))
	int32 CurrentPlayersCount = 0;

	// Max players count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MatchInfo", meta = (AllowPrivateAccess = "true"))
	int32 MaxPlayersCount = 0;

protected:
	// Current Game Phase widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	URichTextBlock* GamePhaseText = nullptr;

	// Player count widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	URichTextBlock* PlayerCountText = nullptr;

	// Team ID widget refernce
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	URichTextBlock* TeamIDText = nullptr;

	// Health bar widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	UHealthBarWidget* HealthBarWidget = nullptr;
#pragma endregion Variables

#pragma region Initialization
// Initialization
public:
	// Constructor 
	UPlayerHUDWidget();
#pragma endregion Initialization

#pragma region Overrides
// Overrides
protected:
	// On initialized function
	virtual void NativeOnInitialized() override;
#pragma endregion Overrides

#pragma region Functions
public:
	// On Phase changed event
	UFUNCTION()
	void OnPhaseChangedEvent(EMatchPhase NewMatchPhase);

	// On player joined event
	UFUNCTION()
	void OnPlayerJoinedEvent(int32 CurrentPlayers);

	// On max player count updated event
	UFUNCTION()
	void OnMaxPlayerCountUpdatedEvent(int32 MaxPlayers);

	// Update current player count text
	UFUNCTION()
	void UpdateCurrentPlayerCountText();
		
#pragma endregion Functions
};
