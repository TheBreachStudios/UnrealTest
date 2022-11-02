// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "ChampionAudioComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALTEST_API UChampionAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	UChampionAudioComponent();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDeathSFX();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBodyHitSFX();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAbilitySFX();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetAudio();

protected:

	virtual void BeginPlay() override;

	void BindToChampionEvents();
	
	UPROPERTY(EditAnywhere)
	USoundCue* DeathSound = nullptr;
	UPROPERTY(EditAnywhere)
	USoundCue* BodyHitSound = nullptr;
	UPROPERTY(EditAnywhere)
	USoundCue* AbilitySound = nullptr;
};
