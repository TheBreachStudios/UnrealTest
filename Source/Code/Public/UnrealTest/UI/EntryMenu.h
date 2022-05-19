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
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="Components")
	UMasterButton* HostSessionButton = nullptr;

	// Button label widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	UMasterButton* FindSessionButton = nullptr;

#pragma endregion Variables

#pragma region Overrides
protected:
// Overrides
	// On initialized function
	virtual void NativeOnInitialized() override;
#pragma endregion Overrides

#pragma region Functions
// Functions
protected:
	// Try create session
	UFUNCTION(Category = "SessionHandling")
	void TryCreateSession();

	// Try find session
	UFUNCTION(Category = "SessionHandling")
	void TryFindSession();

	// Session created event
	UFUNCTION(Category = "SessionHandling")
	void OnSessionCreatedEvent(bool Success);

	// Session joined event
	UFUNCTION(Category = "SessionHandling")
	void OnSessionJoinedEvent(EBPOnJoinSessionCompleteResult Result);
#pragma endregion Functions
};
