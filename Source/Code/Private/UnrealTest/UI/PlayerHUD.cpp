// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/PlayerHUD.h"
#include "UnrealTest/UI/CharacterHudWidget.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"

APlayerHUD::APlayerHUD() 
{
	static ConstructorHelpers::FClassFinder<UCharacterHudWidget> UserWidgetBPClass(TEXT("/Game/UI/HUD/BP_CharacterHUDWidget"));
	if (UserWidgetBPClass.Class != NULL)
	{
		HudWidgetClass = UserWidgetBPClass.Class;
	}
}

void APlayerHUD::BeginPlay() 
{
	Super::BeginPlay();

	//Instantiate HUD Widget
	if (HudWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UCharacterHudWidget>(GetWorld(), HudWidgetClass, FName(TEXT("PlayerHUD")));
		if (CurrentWidget != nullptr) 
		{ 
			CurrentWidget->AddToViewport();
		}
	}
}
