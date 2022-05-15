#include "UnrealTest/UI/MasterButton.h"

// Unreal Engine
#include "Components/RichTextBlock.h"
#include "Components/Button.h"

#pragma region Overrides
// Overrides

// On initialized function
void UMasterButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// Bind actual button to MasterButton OnButtonClicked event
	Button->OnClicked.AddUniqueDynamic(this, &UMasterButton::OnButtonClickedEvent);
}

// Pre construct function
void UMasterButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Set button label
	ButtonLabel->SetText(ButtonLabelContent);
}
#pragma endregion Overrides

#pragma region Getters / Setters
// Get ButtonLabel content
FText UMasterButton::GetButtonLabelContent() 
{
	return ButtonLabel->GetText();
}

// Set ButtonLabel content
void UMasterButton::SetButtonLabelContent(FText LabelContent)
{
	ButtonLabelContent = LabelContent;
	ButtonLabel->SetText(ButtonLabelContent);
}
#pragma endregion Getters / Setters

#pragma region Functions
// Functions

// Broadcasts Button OnClicked event
void UMasterButton::OnButtonClickedEvent()
{
	OnButtonClicked.Broadcast();
}
#pragma endregion Functions