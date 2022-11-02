// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "ChampionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AChampionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FChampionSignature, APlayerController*);

public:
	
	UFUNCTION(Client, Reliable)
	void Client_LockPlayerInput(APlayerController* playerCtrl);
	UFUNCTION(Client, Reliable)
	void Client_UnlockPlayerInput(APlayerController* playerCtrl);
	
	FChampionSignature OnPlayerDeathEvent;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(Client, Reliable)
	void Client_HandleChampionDeath();
	UFUNCTION(Server, Reliable)
	void Server_HandleChampionDeath();

	void BindCharacterEvents();
	void UnbindCharacterEvents();

	void ResetChampionCharacter();

	AChampionCharacter* ChampionPawn;

	FDelegateHandle ChampionDeathDelegateHandle_Server;
	FDelegateHandle ChampionDeathDelegateHandle_Client;
};
