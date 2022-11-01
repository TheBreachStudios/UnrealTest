// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Interfaces/IDamageable.h"
#include "ChampionCharacter.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class UNREALTEST_API AChampionCharacter : public AUnrealTestCharacter, public IDamageable
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE(FChampionSignature);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FHealthChangedSignature, float, float);
	
public :
	AChampionCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void ReloadBinding(class UInputComponent* PlayerInputComponent);
	void HandleDeath();
	void TryFindWeapon();
	void ResetCurrentHealth();

	UFUNCTION(Client, Reliable)
	void Client_BroadcastHealthChanged();

	UFUNCTION()
	void ShootingStarted();

	UFUNCTION()
	void ShootingStopped();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void OnRepCurrentHealth();
	
	//class UHealthComponent* HealthComponent = nullptr;
	class UChampionAnimHandlerComp* AnimHandler = nullptr;
	class UChampionAudioComponent* AudioComponent = nullptr;
	class ABaseWeapon* Weapon = nullptr;

	UPROPERTY(ReplicatedUsing = OnRepCurrentHealth)
	float CurrentHealth = 0.f;

	const float MAX_HEALTH = 100.f;

public:
	//IDamageable
	virtual void ApplyDamage(float damage) override;
	virtual bool CanReceiveDamage() override;
	virtual void Destroy() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetChampionCharacter();

	FORCEINLINE float GetMaxHealth() const { return MAX_HEALTH; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE const ABaseWeapon* GetWeapon() const { return Weapon; }
	FORCEINLINE ABaseWeapon* AccessWeapon() { return Weapon; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UChampionAnimHandlerComp* AccessAnimationHandler() { return AnimHandler; }

	FHealthChangedSignature OnHealthChangedEvent;
	FChampionSignature OnDamagedEvent;
	FChampionSignature OnChampionDeathEvent;
	FChampionSignature OnWeaponAquiredEvent;
};
