// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Weapons/WeaponAudioComponent.h"
#include "ShootingWeaponAudioComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALTEST_API UShootingWeaponAudioComponent : public UWeaponAudioComponent
{
	GENERATED_BODY()

public:
	UShootingWeaponAudioComponent();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayReloadSFX();

protected:
	UPROPERTY(EditAnywhere)
	class USoundCue* ReloadSound = nullptr;
};
