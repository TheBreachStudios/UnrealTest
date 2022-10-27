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
	virtual void Reload();
	virtual void ResetClipAmmo();

	UPROPERTY(EditAnywhere)
	int MaxReserveAmmo = 0;

	UPROPERTY(EditAnywhere)
	int MaxClipAmmo = 0;

	UPROPERTY()
	int CurrentClipAmmo = 0;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.f;

	UPROPERTY(EditAnywhere)
	float ReloadingTime = 0.f;
};
