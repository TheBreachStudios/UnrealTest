// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "CharacterHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API UCharacterHudWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

//public:
//	UFUNCTION(BlueprintImplementableEvent)
//		void UpdateHp();
//
//	UFUNCTION(BlueprintImplementableEvent)
//		void UpdateClipAmmo();
//
//	UFUNCTION(BlueprintImplementableEvent)
//		void UpdateReserveAmmo();

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
