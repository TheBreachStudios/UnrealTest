// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

// Game Project
#include "UnrealTestHUD.generated.h"

// Definitions
class UPlayerHUDWidget;

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestHUD : public AHUD
{
	GENERATED_BODY()

#pragma region Configuration
// Configuration
public:
	// Entry menu template widget
	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetTemplate;
#pragma endregion Configuration

#pragma region Variables
// Variables
protected:
	// Entry Menu reference
	UPlayerHUDWidget* PlayerHUDWidget;
#pragma endregion Variables

#pragma region Overrides
// Overrides

	// Begin play
	virtual void BeginPlay() override;
#pragma endregion Overrides

};
