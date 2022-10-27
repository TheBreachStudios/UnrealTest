// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALTEST_API ShootingWeaponBase
{
public:
	ShootingWeaponBase();
	~ShootingWeaponBase();

protected:
	virtual void UseWeapon() = 0;
	virtual void Reload() = 0;
	virtual void ResetClipAmmo() = 0;
};
