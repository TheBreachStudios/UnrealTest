// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/HealthBarWidget.h"

// Unreal Engine
#include "Components/ProgressBar.h"

// GameProject
#include "UnrealTest/Components/HealthComponent.h"

#pragma region Overrides
void UHealthBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (HealthProgressBar)
	{
		HealthProgressBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
	}
}
#pragma endregion Overrides

#pragma region Functions
// Function binded to health progress var percentage
float UHealthBarWidget::GetHealthPercentage()
{
	return HealthComponent->GetNormalizedHealth();
}

#pragma endregion Functions