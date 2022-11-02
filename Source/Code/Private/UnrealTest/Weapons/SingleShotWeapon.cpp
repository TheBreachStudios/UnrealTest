// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/SingleShotWeapon.h"
#include "UnrealTest/Weapons/ShootingWeaponAudioComponent.h"
#include "UnrealTest/Weapons/WeaponVFXComponent.h"

ASingleShotWeapon::ASingleShotWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//WeaponAudioComponent = FindComponentByClass<UShootingWeaponAudioComponent>();
	//WeaponVFXComponent = FindComponentByClass<UWeaponVFXComponent>();
	WeaponAudioComponent = ObjectInitializer.CreateDefaultSubobject<UShootingWeaponAudioComponent>(this, TEXT("ShootingWeaponAudioComponent"));
	WeaponVFXComponent = ObjectInitializer.CreateDefaultSubobject<UWeaponVFXComponent>(this, TEXT("WeaponVFXComponent"));

	Damage = 10.f;
	Range = 10000.f;
	AttackRate = 3;
	MaxClipAmmo = 30;
	CurrentClipAmmo = MaxClipAmmo;
	MaxReserveAmmo = 100;
	CurrentReserveAmmo = MaxReserveAmmo;
	AmmoUsedPerShot = 1;
	ReloadTime = 3.3f;
}

void ASingleShotWeapon::TryUseWeapon()
{
	if (CanUseWeapon())
	{
		Super::TryUseWeapon();

		if (WeaponAudioComponent != nullptr)
		{
			WeaponAudioComponent->Multicast_PlayAttackSFX();
		}

		if (WeaponVFXComponent != nullptr)
		{
			WeaponVFXComponent->Multicast_PlayAttackVFX_Implementation();
		}
		Server_TraceHitscanShot();
	}
}

void ASingleShotWeapon::TryStartReload()
{
	if (CanReload())
	{
		Super::TryStartReload();

		if (WeaponAudioComponent != nullptr)
		{
			WeaponAudioComponent->Multicast_PlayReloadSFX();
		}
	}
}

void ASingleShotWeapon::ResetWeapon()
{
	Super::ResetWeapon();
	//TODO: Reset all components
	if (WeaponAudioComponent != nullptr)
	{
		WeaponAudioComponent->Multicast_ResetAudio();
	}	

	if (WeaponVFXComponent != nullptr) 
	{
		WeaponVFXComponent->Multicast_ResetVFX();
	}
}
