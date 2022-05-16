// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/UI/MasterWidget.h"
#include "MasterButton.generated.h"

// Definitions
class URichTextBlock;
class UButton;

/**
 * Master widget for buttons
 */
UCLASS()
class UNREALTEST_API UMasterButton : public UMasterWidget
{
	GENERATED_BODY()

#pragma region Delegates
// Delegates
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);
	UPROPERTY(BlueprintAssignable, Category = "ButtonSettings")
	FOnButtonClicked OnButtonClicked;

#pragma endregion Delegates

#pragma region Configuration
// Configuration
protected:
#define LOCTEXT_NAMESPACE "TechTest"
	
	// Button label content
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonSettings")
	FText ButtonLabelContent = LOCTEXT("ButtonPlaceholder", "Button");
#undef LOCTEXT_NAMESPACE
#pragma endregion Configuration

#pragma region Variables
// Variables
protected:
	// Button widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	UButton* Button = nullptr;

	// Button label widget reference
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	URichTextBlock* ButtonLabel = nullptr;

#pragma endregion Variables

#pragma region Getters / Setters
// Getters / Setters
public:
	// Get ButtonLabel content
	UFUNCTION(BlueprintCallable, Category = "ButtonSettings")
	FText GetButtonLabelContent();

	// Set ButtonLabel content
	UFUNCTION(BlueprintCallable, Category = "ButtonSettings")
	void SetButtonLabelContent(FText LabelContent);

#pragma endregion Getters / Setters

#pragma region Overrides
// Overrides
protected:
	// On initialized function
	virtual void NativeOnInitialized() override;

	// Pre construct function
	virtual void NativePreConstruct() override;
#pragma endregion Overrides

#pragma region Functions
// Functions
protected:
	// Broadcasts Button OnClicked event
	UFUNCTION(Category = "Events")
	void OnButtonClickedEvent();
#pragma endregion Functions
};
