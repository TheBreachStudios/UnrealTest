// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/UnrealTestHUD.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"

// GameProject
#include "Code/Public/UnrealTest/UI/PlayerHUDWidget.h"
#include "Code/Public/UnrealTest/UI/GameOverWidget.h"

#pragma region Getters / Setters
// Set Healthbar widget health component.
void AUnrealTestHUD::SetHealthComponent(UHealthComponent* HealthComponent)
{
	PlayerHUDWidget->SetHealthComponent(HealthComponent);
}
#pragma endregion Getters / Setters

#pragma region Overrides
// Called when the game starts.
void AUnrealTestHUD::BeginPlay()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) { return; }

	AddPlayerHUDWidget();
	AddGameOverWidget();
}
#pragma endregion Overrides

#pragma region Functions
// Adds player HUD widget. It creates, if it did not not exist.
void AUnrealTestHUD::AddPlayerHUDWidget()
{
	if (!PlayerHUDWidget) {
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDWidgetTemplate);

	}

	PlayerHUDWidget->AddToViewport(PlayerHUDWidgetZOrder);
}

// Adds game over widget. It creates, if it did not not exist.
void AUnrealTestHUD::AddGameOverWidget()
{
	if (!GameOverWidget) {
		GameOverWidget = CreateWidget<UGameOverWidget>(PlayerController, GameOverWidgetTemplate);
	}

	UpdateGameOverWidgetVisibility(false);
	GameOverWidget->AddToViewport(GameOverWidgetZOrder);
}

// Updates Player HUD widget visibility
void AUnrealTestHUD::UpdatePlayerHUDWidgetVisibility(bool bNewVisibility)
{
	PlayerHUDWidget->SetVisibility(bNewVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

// Updates Defeated Team ID on GameOver widget.
void AUnrealTestHUD::UpdateDefeatedTeamID(int32 DefeatedTeamID)
{
	GameOverWidget->UpdateDefeatedTeamID(DefeatedTeamID);
}

// Updates Game Over widget visibility.
void AUnrealTestHUD::UpdateGameOverWidgetVisibility(bool bNewVisibility)
{
	GameOverWidget->SetVisibility(bNewVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
#pragma endregion Functions
