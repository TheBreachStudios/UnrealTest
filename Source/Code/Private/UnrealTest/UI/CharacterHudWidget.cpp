// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/CharacterHudWidget.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Game/EliminationGameState.h"

void UCharacterHudWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AChampionCharacter* player = GetOwningPlayerPawn<AChampionCharacter>();
	if (player != nullptr) 
	{
		player->OnHealthChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateHealth);
		UpdateHealth(player->GetCurrentHealth(), player->GetMaxHealth());

		player->OnOwnTeamLivesChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateOwnTeamLives);
		player->OnEnemyTeamLivesChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateEnemyTeamLives);
		AEliminationGameState* gameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AEliminationGameState>() : nullptr;
		if (gameState != nullptr)
		{			
			UpdateOwnTeamLives(gameState->GetMaxLives());
			UpdateEnemyTeamLives(gameState->GetMaxLives());
		}

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

void UCharacterHudWidget::SetHealthTextAndBar(float currentHealth, float maxHealth)
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

void UCharacterHudWidget::SetClipAmmoText(int32 currentClipAmmo, int32 maxClipAmmo)
{
	if (ClipAmmoText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d / %d"), currentClipAmmo, maxClipAmmo);
		ClipAmmoText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::SetReserveAmmoText(int32 reserveAmmo)
{
	if (ReserveAmmoText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d"), reserveAmmo);
		ReserveAmmoText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::SetOwnTeamLivesText(int32 lives)
{
	if (TeamLivesText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d"), lives);
		TeamLivesText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::SetEnemyTeamLivesText(int32 lives)
{
	if (EnemyTeamLivesText != nullptr)
	{
		FString str = FString::Printf(TEXT("%d"), lives);
		EnemyTeamLivesText->SetText(FText::FromString(str));
	}
}

void UCharacterHudWidget::UpdateHealth(float currentHealth, float maxHealth)
{
	SetHealthTextAndBar(currentHealth, maxHealth);
}

void UCharacterHudWidget::UpdateClipAmmo(int32 currentClipAmmo, int32 maxClipAmmo)
{
	SetClipAmmoText(currentClipAmmo, maxClipAmmo);
}

void UCharacterHudWidget::UpdateReserveAmmo(int32 currentReserveAmmo)
{
	SetReserveAmmoText(currentReserveAmmo);
}

void UCharacterHudWidget::UpdateOwnTeamLives(int32 lives)
{
	SetOwnTeamLivesText(lives);
}

void UCharacterHudWidget::UpdateEnemyTeamLives(int32 lives)
{
	SetEnemyTeamLivesText(lives);
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
				weapon->OnClipAmmoChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateClipAmmo);
				UpdateClipAmmo(weapon->GetCurrentClipAmmo(), weapon->GetMaxClipAmmo());
				weapon->OnReserveAmmoChangedEvent.AddUObject(this, &UCharacterHudWidget::UpdateReserveAmmo);
				UpdateReserveAmmo(weapon->GetCurrentReserveAmmo());
			}
		}
	}
}

