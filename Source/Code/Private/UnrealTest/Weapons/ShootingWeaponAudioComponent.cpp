// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/ShootingWeaponAudioComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UShootingWeaponAudioComponent::UShootingWeaponAudioComponent()
{
	// TEMP
	static ConstructorHelpers::FObjectFinder<USoundCue> ReloadSoundCue(TEXT("/Game/Audio/Weapons/Reload_SoundCue"));
	if (ReloadSoundCue.Succeeded())
	{
		ReloadSound = ReloadSoundCue.Object;
	}
}

void UShootingWeaponAudioComponent::Multicast_PlayReloadSFX_Implementation()
{
	if (ReloadSound != nullptr)
	{
		SetSound(ReloadSound);
		Play();
	}
}
