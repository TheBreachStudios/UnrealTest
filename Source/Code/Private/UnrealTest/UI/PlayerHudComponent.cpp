// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/UI/PlayerHudComponent.h"
#include "Blueprint/UserWidget.h"
#include "UnrealTest/UI/CharacterHudWidget.h"

UPlayerHudComponent::UPlayerHudComponent()
{
}


void UPlayerHudComponent::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* owningPawn = Cast<APawn>(GetOwner());
	if (owningPawn == nullptr) { return; }
	
	if (owningPawn->IsLocallyControlled() && PlayerHudClass != nullptr)
	{
		APlayerController* playerController = owningPawn->GetController<APlayerController>();
		if (playerController == nullptr) { return; }

		PlayerHUD = CreateWidget<UCharacterHudWidget>(playerController, PlayerHudClass, FName(TEXT("PlayerHUD")));
		if (PlayerHUD == nullptr) { return; }
		PlayerHUD->AddToPlayerScreen();
	}
}

void UPlayerHudComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD != nullptr)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}


