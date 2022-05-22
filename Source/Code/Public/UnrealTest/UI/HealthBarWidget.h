// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/UI/MasterWidget.h"
#include "HealthBarWidget.generated.h"

// Definitions
class UHealthComponent;
class UProgressBar;

/**
 * 
 */
UCLASS()
class UNREALTEST_API UHealthBarWidget : public UMasterWidget
{
	GENERATED_BODY()

#pragma region Variables
// Variables
protected:
	// Button widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	UProgressBar* HealthProgressBar = nullptr;

public:
	// Health component reference
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (ExposeOnSpawn = "true"));
	UHealthComponent* HealthComponent;


#pragma endregion Variables

#pragma region Override
// Override
protected:
	// On initialized function.
	virtual void NativeOnInitialized() override;
#pragma endregion Override

#pragma region Functions
// Functions
protected:
	// Function binded to health progress var percentage
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bindings")
	float GetHealthPercentage();
#pragma endregion Functions

};
