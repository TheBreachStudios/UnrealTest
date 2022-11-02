// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponAudioComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UWeaponAudioComponent::UWeaponAudioComponent() 
{
	// TEMP
	if (AttackSound == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundCue(TEXT("/Game/Audio/Weapons/RifleShot_SoundCue"));
		if (AttackSoundCue.Succeeded())
		{
			AttackSound = AttackSoundCue.Object;
		}
	}	
}

void UWeaponAudioComponent::Multicast_PlayAttackSFX_Implementation()
{
	if(AttackSound != nullptr)
	{
		SetSound(AttackSound);
		Play();
	}
}

void UWeaponAudioComponent::Multicast_ResetAudio_Implementation()
{
	//TODO: Stop all playing sounds.
}
