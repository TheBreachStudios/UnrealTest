// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/CharacterHudWidget.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Character/ChampionCharacter.h"



void UCharacterHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind to the Lives changes
	AChampionCharacter* player = GetOwningPlayerPawn<AChampionCharacter>();
	if (player != nullptr) 
	{
		player->OnHealthChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateHealth);
		UpdateHealth(player->GetCurrentHealth(), player->GetMaxHealth());

		if (player->GetWeapon() != nullptr)
		{
			TryBindToWeaponEvents();
		}
		else 
		{
			player->OnWeaponAquiredEvent.AddUObject(this, &UCharacterHudWidget::TryBindToWeaponEvents);
		}
	}
}

void UCharacterHudWidget::SetHealth(float currentHealth, float maxHealth)
{
	if (HpText != nullptr)
	{
		int32 intCurrentHealth = FMath::CeilToInt(currentHealth);
		int32 intMaxHealth = FMath::CeilToInt(maxHealth);
		FString str = FString::Printf(TEXT("%d / %d"), intCurrentHealth, intMaxHealth);
		HpText->SetText(FText::FromString(str));
	}

	if (HpProgressBar != nullptr)
	{
		const float normalizedHealth = maxHealth > 0 ? (float)currentHealth / maxHealth : 0.f;
		HpProgressBar->SetPercent(normalizedHealth);
	}
}

void UCharacterHudWidget::SetClipAmmo(int32 currentClipAmmo, int32 maxClipAmmo)
{
	if (ClipAmmoText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d / %d"), currentClipAmmo, maxClipAmmo);
		ClipAmmoText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::SetReserveAmmo(int32 reserveAmmo)
{
	if (ReserveAmmoText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d"), reserveAmmo);
		ReserveAmmoText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::SetTeamLives(int32 onwTeamLives, int32 enemyTeamLives)
{
	if (TeamLivesText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d"), onwTeamLives);
		TeamLivesText->SetText(FText::FromString(str));
	}

	if (EnemyTeamLivesText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d"), enemyTeamLives);
		EnemyTeamLivesText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::UpdateHealth(float currentHealth, float maxHealth)
{
	SetHealth(currentHealth, maxHealth);
}

void UCharacterHudWidget::UpdateAmmo(int32 currentClipAmmo, int32 maxClipAmmo, int32 currentReserveAmmo)
{
	SetClipAmmo(currentClipAmmo, maxClipAmmo);
	SetReserveAmmo(currentReserveAmmo);
}

void UCharacterHudWidget::UpdateLives()
{
	//TODO
}

void UCharacterHudWidget::TryBindToWeaponEvents()
{
	AChampionCharacter* player = GetOwningPlayerPawn<AChampionCharacter>();
	if (player != nullptr)
	{
		if (player->GetWeapon() != nullptr)
		{
			ABaseShootingWeapon* weapon = Cast<ABaseShootingWeapon>(player->AccessWeapon());
			if (weapon != nullptr)
			{
				weapon->OnAmmoChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateAmmo);
				UpdateAmmo(weapon->GetCurrentClipAmmo(), weapon->GetMaxClipAmmo(), weapon->GetCurrentReserveAmmo());
			}
		}
	}
}

