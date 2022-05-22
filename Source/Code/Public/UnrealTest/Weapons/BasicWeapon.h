// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// Game Project
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "BasicWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API ABasicWeapon : public ABaseWeapon
{
	GENERATED_BODY()
#pragma region Initialization
// Initialization
public:
	// Sets default values for this actor's properties
	ABasicWeapon();
#pragma endregion Initialization
};
