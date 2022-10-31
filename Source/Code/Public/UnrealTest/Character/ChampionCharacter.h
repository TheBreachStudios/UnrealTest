// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "ChampionCharacter.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class UNREALTEST_API AChampionCharacter : public AUnrealTestCharacter
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE(FChampionSignature);
	
public :
	AChampionCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void ReloadBinding(class UInputComponent* PlayerInputComponent);
	void SetupHealthComponent();
	void HandleDeath();
	void TryFindWeapon();

	UFUNCTION()
	void ShootingStarted();

	UFUNCTION()
	void ShootingStopped();

	UFUNCTION()
	void Reload();

	//TEMP
	UFUNCTION(Server, Reliable)
	void Server_DoHitScanTrace();

	//TEMP
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayShotVFX();

	class UHealthComponent* HealthComponent = nullptr;
	class UChampionAnimHandlerComp* AnimHandler = nullptr;
	class UChampionAudioComponent* AudioComponent = nullptr;
	class ABaseWeapon* Weapon = nullptr;

	bool HasWeapon = false;

public:

	FORCEINLINE const UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE const ABaseWeapon* GetWeapon() const { return Weapon; }
	FORCEINLINE ABaseWeapon* AccessWeapon() { return Weapon; }

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetChampionCharacter();

	FChampionSignature OnChampionDeathEvent;

	class USoundCue* SoundDeath = nullptr;
	class USoundCue* SoundShot = nullptr;

	class UParticleSystem* ShotVFX = nullptr;
};
