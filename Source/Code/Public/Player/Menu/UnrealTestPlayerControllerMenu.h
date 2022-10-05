// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSessionSettings.h"
#include "OnlineError.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UnrealTestPlayerControllerMenu.generated.h"

class AUnrealTestGameSession;

/**
 * Base class for main menu player controller
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerControllerMenu : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaSeconds) override;

	AUnrealTestGameSession* GetGameSession() const;
	
};
