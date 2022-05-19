// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/UnrealTestHUD.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"

// GameProject
#include "Code/Public/UnrealTest/UI/PlayerHUDWidget.h"
#include "Code/Public/UnrealTest/UI/GameOverWidget.h"

#pragma region Overrides
// Begin play
void AUnrealTestHUD::BeginPlay()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) { return; }

	AddPlayerHUDWidget();
	AddGameOverWidget();
}
#pragma endregion Overrides

#pragma region Functions
void AUnrealTestHUD::AddPlayerHUDWidget()
{
	if (!PlayerHUDWidget) {
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDWidgetTemplate);

	}

	PlayerHUDWidget->AddToViewport(PlayerHUDWidgetZOrder);
}

void AUnrealTestHUD::AddGameOverWidget()
{
	if (!GameOverWidget) {
		GameOverWidget = CreateWidget<UGameOverWidget>(PlayerController, GameOverWidgetTemplate);
	}

	UpdateGameOverWidgetVisibility(false);
	GameOverWidget->AddToViewport(GameOverWidgetZOrder);
}

void AUnrealTestHUD::UpdatePlayerHUDWidgetVisibility(bool bNewVisibility)
{
	PlayerHUDWidget->SetVisibility(bNewVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void AUnrealTestHUD::UpdateGameOverWidgetVisibility(bool bNewVisibility)
{
	GameOverWidget->SetVisibility(bNewVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
#pragma endregion Functions
