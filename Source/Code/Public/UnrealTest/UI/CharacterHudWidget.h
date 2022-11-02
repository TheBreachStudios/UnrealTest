// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Net/UnrealNetwork.h"
#include "CharacterHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API UCharacterHudWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	void SetHealthTextAndBar(float currentHealth, float maxHealth);
	void SetClipAmmoText(int32 currentClipAmmo, int32 maxClipAmmo);
	void SetReserveAmmoText(int32 reserveAmmo);
	void SetOwnTeamLivesText(int32 lives);
	void SetEnemyTeamLivesText(int32 lives);

	void UpdateHealth(float currentHealth, float maxHealth);
	void UpdateClipAmmo(int32 currentClipAmmo, int32 maxClipAmmo);
	void UpdateReserveAmmo(int32 currentReserveAmmo);
	void UpdateOwnTeamLives(int32 lives);
	void UpdateEnemyTeamLives(int32 lives);

	void TryBindToWeaponEvents();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* HpText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ClipAmmoText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ReserveAmmoText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TeamLivesText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* EnemyTeamLivesText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HpProgressBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* CharacterPortrait = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* WeaponImage = nullptr;
};
