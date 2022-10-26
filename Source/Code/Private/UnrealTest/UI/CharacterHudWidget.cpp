// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/CharacterHudWidget.h"

void UCharacterHudWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	// Bind delegates here.
}

void UCharacterHudWidget::SetHealth(int currentHealth, int maxHealth)
{
	if (HpText != nullptr)
	{		
		HpText->SetText(FText::FromString(currentHealth + " / " + maxHealth));
	}

	if (HpProgressBar != nullptr) 
	{
		const float normalizedHealth = maxHealth > 0 ? (float)currentHealth / maxHealth : 0.f;
		HpProgressBar->SetPercent(normalizedHealth);
	}
}

void UCharacterHudWidget::SetClipAmmo(int currentClipAmmo, int maxClipAmmo)
{
	if (ClipAmmoText != nullptr)
	{
		ClipAmmoText->SetText(FText::FromString(currentClipAmmo + " / " + maxClipAmmo));
	}
}

void UCharacterHudWidget::SetReserveAmmo(int reserveAmmo)
{
	if (ReserveAmmoText != nullptr)
	{
		ReserveAmmoText->SetText(FText::AsNumber(reserveAmmo));
	}
}

