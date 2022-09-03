// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Weapon/UnrealTestWeaponBase.h"
#include "UnrealTestGunBase.generated.h"

/**
* Abstract class representing a gun
*/
UCLASS(Abstract)
class UNREALTEST_API AUnrealTestGunBase : public AUnrealTestWeaponBase
{
	GENERATED_BODY()
};
