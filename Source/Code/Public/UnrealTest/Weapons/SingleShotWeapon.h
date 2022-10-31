// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Weapons/ShootingWeaponAudioComponent.h"
#include "SingleShotWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API ASingleShotWeapon : public ABaseShootingWeapon
{
	GENERATED_BODY()
	
public:
	ASingleShotWeapon();

	virtual void TryUseWeapon() override;
	virtual void TryStartReload() override;
	virtual void ResetWeapon() override;

protected:
	
	class UShootingWeaponAudioComponent* AudioComponent = nullptr;
};
