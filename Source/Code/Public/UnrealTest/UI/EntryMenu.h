// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/UI/MasterWidget.h"
#include "EntryMenu.generated.h"

// Definitions
class UMasterButton;
class UUnrealTestGameInstanceSubsystem;
class AUnrealTestGameMode;
struct FBlueprintSessionResult;

/**
 * Entry menu
 */
UCLASS()
class UNREALTEST_API UEntryMenu : public UMasterWidget
{
	GENERATED_BODY()

#pragma region Variables
// Vriables
private:
	// Game Instance Subsystem reference
	UUnrealTestGameInstanceSubsystem* GameInstanceSubsystem = nullptr;

	// Game mode reference
	AUnrealTestGameMode* GameMode;

protected:
	// Button widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMasterButton* HostSessionButton = nullptr;

	// Button label widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMasterButton* FindSessionButton = nullptr;

#pragma endregion Variables

#pragma region Overrides
// Overrides
	// On initialized function
	virtual void NativeOnInitialized() override;
#pragma endregion Overrides

#pragma region Functions
// Functions
	// Try create session
	UFUNCTION()
	void TryCreateSession();

	// Try find session
	UFUNCTION()
	void TryFindSession();

	// Session created event
	UFUNCTION()
	void OnSessionCreatedEvent(bool Success);

	// Session joined event
	UFUNCTION()
	void OnSessionJoinedEvent(EBPOnJoinSessionCompleteResult Result);
#pragma endregion Functions
};
