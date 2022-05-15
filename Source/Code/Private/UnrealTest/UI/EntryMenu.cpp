// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/EntryMenu.h"

// Unreal Engine
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FindSessionsCallbackProxy.h"
#include "OnlineSubsystemUtils.h"

// Game Project
#include "Code/Public/UnrealTest/UI/MasterButton.h"
#include "Code/Public/UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"
#include "Code/Public/UnrealTest/Game/UnrealTestGameMode.h"

#pragma region Overrides
void UEntryMenu::NativeOnInitialized()
{
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	GameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();

	// Bind game instance events
	GameInstanceSubsystem->OnCreateSessionCompleted.AddDynamic(this, &UEntryMenu::OnSessionCreatedEvent);
	GameInstanceSubsystem->OnJoinGameSessionCompleted.AddDynamic(this, &UEntryMenu::OnSessionJoinedEvent);

	// Bind button events
	HostSessionButton->OnButtonClicked.AddUniqueDynamic(this, &UEntryMenu::TryCreateSession);
	FindSessionButton->OnButtonClicked.AddUniqueDynamic(this, &UEntryMenu::TryFindSession);
}
#pragma endregion Overrides

#pragma region Functions
// Try create session
void UEntryMenu::TryCreateSession()
{
	GameMode = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode) {
		UE_LOG(LogTemp, Fatal, TEXT("GameMode is not AUnrealTestGameMode."));
	}
	GameInstanceSubsystem->CreateSession(GameMode->GetMaxPlayerPerSession(), false);
}

// Try find session
void UEntryMenu::TryFindSession()
{
	GameMode = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode) {
		UE_LOG(LogTemp, Fatal, TEXT("GameMode is not AUnrealTestGameMode."));
	}
	GameInstanceSubsystem->FindSessions(GameMode->GetMaxSessionSearchResults(), false);
}

// Session created event
void UEntryMenu::OnSessionCreatedEvent(bool Success)
{
	if (Success)
	{
		// TODO: Move values to data asset
		UGameplayStatics::OpenLevel(this, TEXT("TestLevel"), false, "?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session couldn't be created."))
	}
}

// Session joined event
void UEntryMenu::OnSessionJoinedEvent(EBPOnJoinSessionCompleteResult Result)
{
	if (Result == EBPOnJoinSessionCompleteResult::Success)
	{
		GameInstanceSubsystem->TryTravelToCurrentSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session couldn't be joined. Result: %s."), *UEnum::GetValueAsString(Result))
	}
}
#pragma endregion Functions