// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "WeaponAudioComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALTEST_API UWeaponAudioComponent : public UAudioComponent
{
	GENERATED_BODY()
	
public:
	UWeaponAudioComponent();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackSFX();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetAudio();

protected:
	UPROPERTY(EditAnywhere)
	class USoundCue* AttackSound = nullptr;

};
