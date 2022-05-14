// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Game Project
#include "UnrealTestGameMode.generated.h"

UCLASS(minimalapi)
class AUnrealTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
#pragma region Initialization
	// Initialization
	AUnrealTestGameMode();
#pragma endregion Initialization


#pragma region Functions
	// Creates a session a host it
	UFUNCTION(BlueprintCallable)
	void HostGame();

	// Joins the first session available
	UFUNCTION(BlueprintCallable)
	void JoinGame();
#pragma endregion Functions
};