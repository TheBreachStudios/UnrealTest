// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAudioComponent.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UChampionAudioComponent::UChampionAudioComponent()
{
	if (DeathSound == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USoundCue> DeathSoundCue(TEXT("/Game/Audio/Characters/Death_SoundCue"));
		if (DeathSoundCue.Succeeded())
		{
			DeathSound = DeathSoundCue.Object;
		}
	}

	if (BodyHitSound == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USoundCue> BodyHitSoundCue(TEXT("/Game/Audio/Characters/BodyHit_SoundCue"));
		if (BodyHitSoundCue.Succeeded())
		{
			BodyHitSound = BodyHitSoundCue.Object;
		}
	}

	if (AbilitySound == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USoundCue> AbilitySoundCue(TEXT("/Game/StarterContent/Audio/Explosion_Cue"));
		if (AbilitySoundCue.Succeeded())
		{
			AbilitySound = AbilitySoundCue.Object;
		}
	}
}

void UChampionAudioComponent::Multicast_PlayDeathSFX_Implementation()
{
	if (DeathSound != nullptr)
	{
		SetSound(DeathSound);
		Play();
	}
}

void UChampionAudioComponent::Multicast_PlayBodyHitSFX_Implementation()
{
	if (BodyHitSound != nullptr)
	{
		SetSound(BodyHitSound);
		Play();
	}
}

void UChampionAudioComponent::Multicast_PlayAbilitySFX_Implementation()
{
	if (AbilitySound != nullptr)
	{
		SetSound(AbilitySound);
		Play();
	}
}

void UChampionAudioComponent::Multicast_ResetAudio_Implementation()
{
	//TODO: Stop all playing sounds.
}

void UChampionAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	BindToChampionEvents();
}

void UChampionAudioComponent::BindToChampionEvents()
{
	if (GetOwner() == nullptr) { return; }

	AChampionCharacter* champion = Cast<AChampionCharacter>(GetOwner());
	if (champion != nullptr)
	{
		champion->OnDamagedEvent.AddUObject(this, &UChampionAudioComponent::Multicast_PlayBodyHitSFX);
		champion->OnChampionDeathEvent.AddUObject(this, &UChampionAudioComponent::Multicast_PlayDeathSFX);
		champion->OnAbilityUsedEvent.AddUObject(this, &UChampionAudioComponent::Multicast_PlayAbilitySFX);
	}
}


