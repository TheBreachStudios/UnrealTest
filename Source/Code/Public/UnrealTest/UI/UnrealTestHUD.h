// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

// Game Project
#include "UnrealTestHUD.generated.h"

// Definitions
class UPlayerHUDWidget;
class UGameOverWidget;
class UHealthComponent;

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
	// Entry menu template widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetTemplate = nullptr;

	// Entry menu template widget ZOrder.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	int32 PlayerHUDWidgetZOrder = 0;

	// Game over screen widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	TSubclassOf<UGameOverWidget> GameOverWidgetTemplate = nullptr;

	// Game over screen widget ZOrder.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	int32 GameOverWidgetZOrder = 0;

#pragma endregion Configuration

#pragma region Variables
// Variables
private:
	// Player controller reference.
	APlayerController* PlayerController;

protected:
	// Player HUD widget reference.
	UPlayerHUDWidget* PlayerHUDWidget;

	// Game Over widget reference.
	UGameOverWidget* GameOverWidget;
#pragma endregion Variables

#pragma region Getters / Setters
// Getters / Setters
public:
	// Set Healthbar widget health component.
	void SetHealthComponent(UHealthComponent* HealthComponent);
#pragma endregion Getters / Setters

#pragma region Overrides
// Overrides
protected:
	// Begin play.
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region Functions
// Functions
public:
	// Adds player HUD widget. It creates, if it did not not exist.
	void AddPlayerHUDWidget();

	// Adds game over widget. It creates, if it did not not exist.
	void AddGameOverWidget();

	// Updates Player HUD widget visibility
	void UpdatePlayerHUDWidgetVisibility(bool bNewVisibility);

	// Updates Defeated Team ID on GameOver widget.
	void UpdateDefeatedTeamID(int32 DefeatedTeamID);

	// Updates Game Over widget visibility.
	void UpdateGameOverWidgetVisibility(bool bNewVisibility);
#pragma endregion Functions
};
