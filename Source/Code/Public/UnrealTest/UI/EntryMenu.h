// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/UI/MasterWidget.h"
#include "EntryMenu.generated.h"

// Definitions
class UMasterButton;

/**
 * Entry menu
 */
UCLASS()
class UNREALTEST_API UEntryMenu : public UMasterWidget
{
	GENERATED_BODY()
#pragma region Variables
// Variables
protected:
	// Button widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMasterButton* HostSessionButton = nullptr;

	// Button label widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMasterButton* FindSessionButton = nullptr;

#pragma endregion Variables
};
