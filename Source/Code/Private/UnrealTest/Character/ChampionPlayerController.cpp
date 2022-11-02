// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionPlayerController.h"
#include "UnrealTest/Character/ChampionCharacter.h"

void AChampionPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	ChampionPawn = Cast<AChampionCharacter>(aPawn);

	if (ChampionPawn != nullptr)
	{
		BindCharacterEvents();
		ResetChampionCharacter();
	}
}

void AChampionPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	UnbindCharacterEvents();
	ChampionPawn = nullptr;
}

void AChampionPlayerController::BindCharacterEvents()
{
	ChampionDeathDelegateHandle_Client = ChampionPawn->OnChampionDeathEvent.AddUObject(this, &AChampionPlayerController::Client_HandleChampionDeath);
	ChampionDeathDelegateHandle_Server = ChampionPawn->OnChampionDeathEvent.AddUObject(this, &AChampionPlayerController::Server_HandleChampionDeath);
}

void AChampionPlayerController::UnbindCharacterEvents()
{
	ChampionPawn->OnChampionDeathEvent.Remove(ChampionDeathDelegateHandle_Client);
	ChampionPawn->OnChampionDeathEvent.Remove(ChampionDeathDelegateHandle_Server);
}

void AChampionPlayerController::Client_HandleChampionDeath_Implementation()
{
	//TODO: Make sure the input is blocked properly!
	DisableInput(NULL);
}

void AChampionPlayerController::Server_HandleChampionDeath_Implementation()
{
	if (OnPlayerDeathEvent.IsBound())
	{
		OnPlayerDeathEvent.Broadcast(this);
	}
}

void AChampionPlayerController::ResetChampionCharacter()
{
	if (ChampionPawn != nullptr) 
	{
		ChampionPawn->Multicast_ResetChampionCharacter();
	}
}

void AChampionPlayerController::Client_LockPlayerInput_Implementation(APlayerController* player)
{
	if (player->AcknowledgedPawn != nullptr)
	{
		player->AcknowledgedPawn->DisableInput(player);
	}
}

void AChampionPlayerController::Client_UnlockPlayerInput_Implementation(APlayerController* player)
{
	if (player->AcknowledgedPawn != nullptr)
	{
		player->AcknowledgedPawn->EnableInput(player);
	}
}

