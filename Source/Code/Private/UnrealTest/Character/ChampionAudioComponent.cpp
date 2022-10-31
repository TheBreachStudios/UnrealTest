// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAudioComponent.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UChampionAudioComponent::UChampionAudioComponent()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathSoundCue(TEXT("/Game/Audio/Characters/Death_SoundCue"));
	if (DeathSoundCue.Succeeded())
	{
		DeathSound = DeathSoundCue.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> BodyHitSoundCue(TEXT("/Game/Audio/Characters/BodyHit_SoundCue"));
	if (BodyHitSoundCue.Succeeded())
	{
		BodyHitSound = BodyHitSoundCue.Object;
	}

	//static ConstructorHelpers::FObjectFinder<USoundCue> AbilitySoundCue(TEXT("/Game/StarterContent/Audio/Explosion_Cue"));
	//if (AbilitySoundCue.Succeeded())
	//{
	//	AbilitySound = Cast<USoundCue>(AbilitySoundCue);
	//}
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
	//if (AbilitySound != nullptr)
	//{
	//	SetSound(AbilitySound);
	//	Play();
	//}
}

void UChampionAudioComponent::Multicast_ResetAudio_Implementation()
{
	//TODO: Stop all playing sounds.
}

void UChampionAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	BindToHealthEvents();
}

void UChampionAudioComponent::BindToHealthEvents()
{
	if (GetOwner() == nullptr) { return; }

	UHealthComponent* healthComponent = Cast<UHealthComponent>(GetOwner()->GetComponentByClass(UHealthComponent::StaticClass()));
	if (healthComponent != nullptr)
	{
		healthComponent->OnDamagedEvent.AddUObject(this, &UChampionAudioComponent::Multicast_PlayBodyHitSFX);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to bind to the HealthComponent Events!"));
	}
}


