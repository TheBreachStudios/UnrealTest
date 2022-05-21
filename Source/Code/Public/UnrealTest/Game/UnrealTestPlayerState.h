// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UnrealTestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerState : public APlayerState
{
	GENERATED_BODY()

    /* To be used in matchmaking to create teams*/
    UPROPERTY()
        int TeamNumber = 0;

    UPROPERTY()
        int PlayerLevel = 0;

    /* To be obtained from healthcomponent of the player */
    UPROPERTY()
        float Health = 0.f;

    /*
    UPROPERTY()
         EHero CurrentHero = EHero::EXPLOSIVEBALLSHERO;
    */

};
