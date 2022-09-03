// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealTestWeaponBase.generated.h"

/**
* Abstract class representing a weapon
*/

UCLASS(Abstract)
class UNREALTEST_API AUnrealTestWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnrealTestWeaponBase();
};
