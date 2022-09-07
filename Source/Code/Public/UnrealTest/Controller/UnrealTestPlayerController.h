// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealTestPlayerController.generated.h"

class AUnrealTestHeroBase;

/**
 * Unreal test default player controller
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetTeamIndex(int NewIndex);

protected:
	AUnrealTestHeroBase* PossessedHero;

private:
	int TeamIndex = 0;

	virtual void OnPossess(APawn* aPawn) override;
};
