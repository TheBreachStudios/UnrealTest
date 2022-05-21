// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/PlayerHUDWidget.h"

// Unreal Engine
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"

// Game Project
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Game/UnrealTestGameState.h"
#include "UnrealTest/Game/UnrealTestPlayerState.h"
#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"
#include "UnrealTest/Components/HealthComponent.h"
#include "UnrealTest/UI/HealthBarWidget.h"

#pragma region Initialization
// Initialization
// Constructor 
UPlayerHUDWidget::UPlayerHUDWidget() : CurrentMatchPhase(EMatchPhase::NONE) {}
#pragma endregion Initialization

#pragma region Getters / Setters
	// Set health component for Health bar
void UPlayerHUDWidget::SetHealthComponent(UHealthComponent* HealthComponent)
{
	HealthBarWidget->HealthComponent = HealthComponent;
}
#pragma endregion Getters / Setters

#pragma region Overrides
// Overrides

// On initialized function
void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	UUnrealTestGameInstanceSubsystem* gameInstanceSubsystem = gameInstance->GetSubsystem<UUnrealTestGameInstanceSubsystem>();
	CurrentMatchPhase = gameInstanceSubsystem->GetCurrentMatchPhase();

	// Ensures Player HUD Widget widget text is not visible on start
	ESlateVisibility newVisibility = (CurrentMatchPhase == EMatchPhase::NONE) ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;
	SetVisibility(newVisibility);

	GameState = Cast<AUnrealTestGameState>(GetWorld()->GetGameState());
	if (!GameState) { return; }

	TryBindToPlayerStateTeamIDChanged();

	// Bind game state events
	GameState->OnMatchPhaseChanged.AddDynamic(this, &UPlayerHUDWidget::OnPhaseChangedEvent);
	GameState->OnPlayerJoined.AddDynamic(this, &UPlayerHUDWidget::OnPlayerJoinedEvent);
	GameState->OnPlayerMaxChanged.AddDynamic(this, &UPlayerHUDWidget::OnMaxPlayerCountUpdatedEvent);

	// Bind HealtComponent to healtbar
	AUnrealTestCharacter* Character = Cast<AUnrealTestCharacter>(GetOwningPlayerPawn());
	if (Character)
	{
		HealthBarWidget->HealthComponent = Character->GetHealthComponent();
	}

	// Initialization
	OnPhaseChangedEvent(GameState->GetMatchPhase());
	OnPlayerJoinedEvent(GameState->GetCurrentPlayersInSession());
	OnMaxPlayerCountUpdatedEvent(GameState->GetMaxPlayersInSession());
}
#pragma endregion Overrides

#pragma region Functions
// Functions


// Try get Player state
void UPlayerHUDWidget::TryBindToPlayerStateTeamIDChanged()
{
	FTimerHandle Handle;

	AUnrealTestPlayerState* playerState = Cast<AUnrealTestPlayerState>(GetOwningPlayerState());
	if (playerState)
	{
		OnPlayerTeamIDUpdatedEvent(playerState->GetTeamID());
		playerState->OnPlayerTeamIDChanged.AddDynamic(this, &UPlayerHUDWidget::OnPlayerTeamIDUpdatedEvent);
		return;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UPlayerHUDWidget::TryBindToPlayerStateTeamIDChanged, 0.5f, false, 0.f);
	}

}

// On Phase changed event
void UPlayerHUDWidget::OnPhaseChangedEvent(EMatchPhase NewMatchPhase)
{
	CurrentMatchPhase = NewMatchPhase;

	if (NewMatchPhase != EMatchPhase::NONE)
	{
		// Updates Game phase text widget
		GamePhaseText->SetText(FText::FromString(UEnum::GetValueAsString(NewMatchPhase)));
		
		// Show Player HUD Widget
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (NewMatchPhase == EMatchPhase::PLAYING)
	{
		AUnrealTestPlayerState* owningPlayerState = Cast<AUnrealTestPlayerState>(GetOwningPlayerState());
		if (owningPlayerState)
		{
			#define LOCTEXT_NAMESPACE "TechTest"
			TeamIDText->SetText(FText::Format(LOCTEXT("TeamID","{0}"), owningPlayerState->GetTeamID()));
			#undef LOCTEXT_NAMESPACE
		}
	}
}

// On player joined event
void UPlayerHUDWidget::OnPlayerJoinedEvent(int32 CurrentPlayers)
{
	CurrentPlayersCount = CurrentPlayers;
	UpdateCurrentPlayerCountText();
}

// On max player count updated event
void UPlayerHUDWidget::OnMaxPlayerCountUpdatedEvent(int32 MaxPlayers)
{
	MaxPlayersCount = MaxPlayers;
	UpdateCurrentPlayerCountText();
}

// On player team ID updated event
void UPlayerHUDWidget::OnPlayerTeamIDUpdatedEvent(int32 NewTeamID)
{
	TeamID = NewTeamID;
	UpdateTeamIDText();
}

// Update current player count text
void UPlayerHUDWidget::UpdateCurrentPlayerCountText()
{
	#define LOCTEXT_NAMESPACE "TechTest"
	PlayerCountText->SetText(FText::Format(LOCTEXT("CurrentPlayerCountText","{0}/{1}"), CurrentPlayersCount, MaxPlayersCount));
	#undef LOCTEXT_NAMESPACE
}


// Update current player count text
void UPlayerHUDWidget::UpdateTeamIDText()
{
	UE_LOG(LogTemp, Warning, TEXT("[UpdateTeamIDText] FILLING Player: %s Tesm: %i"), *GetOwningPlayer()->GetName(), TeamID);

	#define LOCTEXT_NAMESPACE "TechTest"
	TeamIDText->SetText(FText::Format(LOCTEXT("TeamID", "{0}"), TeamID));
	#undef LOCTEXT_NAMESPACE
}
#pragma endregion Functions