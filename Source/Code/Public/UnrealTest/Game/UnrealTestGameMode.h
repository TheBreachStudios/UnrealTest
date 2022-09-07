// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealTestGameMode.generated.h"

class AUnrealTestPlayerController;

UCLASS(minimalapi)
class AUnrealTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealTestGameMode();

	TArray<AUnrealTestPlayerController*> BlueTeamMembers;
	TArray<AUnrealTestPlayerController*> RedTeamMembers;

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
};
