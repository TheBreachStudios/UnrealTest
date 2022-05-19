// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/UI/MasterWidget.h"
#include "GameOverWidget.generated.h"

// Definitions
class URichTextBlock;

/**
 * 
 */
UCLASS()
class UNREALTEST_API UGameOverWidget : public UMasterWidget
{
	GENERATED_BODY()

#pragma region Variables
// Variables
protected:
	// Button widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	URichTextBlock* GameOverText = nullptr;

#pragma endregion Variables

};
