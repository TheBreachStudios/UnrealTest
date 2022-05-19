// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/HUD/UnrealTestUserWidget.h"
#include "IngameHUD.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREALTEST_API UIngameHUD : public UUnrealTestUserWidget
{
	GENERATED_BODY()

protected:

	/*To show the current character health (healthcomponent)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* HealthText;
	/*To show player's team*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* Team;
	
};
