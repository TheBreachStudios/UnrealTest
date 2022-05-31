// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "UT_CustomPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUT_CustomPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AUT_CustomPlayerStart(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Category = "Team")
	int32 SpawnTeam;

	UPROPERTY(EditAnywhere, Category = "Team")
	bool bIsOccuppied;

};
