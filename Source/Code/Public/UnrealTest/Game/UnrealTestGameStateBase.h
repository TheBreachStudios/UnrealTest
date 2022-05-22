// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTestGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

protected:

	/* Registered matchName */
	UPROPERTY()
		FString MatchGame = "";

	/*
	UPROPERTY()
		ECurrentState GameState = ECurrentState::WAITING;
	*/

	virtual void BeginPlay() override;

	/* TO-DO: Get copy of info from GameMode */
	/*
	bool GetIsMatchEnd();
	bool GetIsMatchInitiated();
	bool GetIsPlayerTeamWinner();
	bool GetIsLastPlayerInTeamAlive();
	*/


};
