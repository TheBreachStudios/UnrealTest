// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/UnrealTestHUD.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"

// GameProject
#include "Code/Public/UnrealTest/UI/PlayerHUDWidget.h"

#pragma region Overrides
// Begin play
void AUnrealTestHUD::BeginPlay()
{

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) { return; }

	PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(playerController, PlayerHUDWidgetTemplate);
	PlayerHUDWidget->AddToViewport();
}
#pragma endregion Overrides