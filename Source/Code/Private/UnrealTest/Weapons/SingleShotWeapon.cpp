// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/SingleShotWeapon.h"
#include "UnrealTest/Weapons/ShootingWeaponAudioComponent.h"
#include "UnrealTest/Weapons/WeaponVFXComponent.h"

ASingleShotWeapon::ASingleShotWeapon()
{
	AudioComponent = CreateDefaultSubobject<UShootingWeaponAudioComponent>(TEXT("ShootingWeaponAudioComponent"));
	VFXComponent = CreateDefaultSubobject<UWeaponVFXComponent>(TEXT("WeaponVFXComponent"));

	//TEMP VALUES
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

		AudioComponent->Multicast_PlayAttackSFX();
		VFXComponent->Multicast_PlayAttackVFX_Implementation();
		Server_TraceHitscanShot();
	}
}

void ASingleShotWeapon::TryStartReload()
{
	if (CanReload())
	{
		Super::TryStartReload();

		AudioComponent->Multicast_PlayReloadSFX();
	}
}

void ASingleShotWeapon::ResetWeapon()
{
	Super::ResetWeapon();
	//TODO: Reset all components
	AudioComponent->Multicast_ResetAudio();
	VFXComponent->Multicast_ResetVFX();
}
