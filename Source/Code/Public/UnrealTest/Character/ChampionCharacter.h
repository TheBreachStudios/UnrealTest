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
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FTeamLivesDelegate, int32);
	DECLARE_MULTICAST_DELEGATE(FChampionDelegate);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FHealthDelegate, float, float);		

public :
	AChampionCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void ReloadBinding(class UInputComponent* PlayerInputComponent);
	void AbilityBinding(class UInputComponent* PlayerInputComponent);
	void HandleDeath();	
	void TryFindWeapon();
	void BindToGameStateEvents();
	void BindToAbilityEvents();

	UFUNCTION()
	void HandleOnAbilityTriggered();

	UFUNCTION()
	void HandleOnTeamLivesChanged(int32 teamID, int32 lives);

	UFUNCTION(Server, Reliable)
	void Server_UseWeapon();

	UFUNCTION(Server, Reliable)
	void Server_ReloadWeapon();

	UFUNCTION(Server, Reliable)
	void Server_UseChampionAbility();

	UFUNCTION(Client, Reliable)
	void Client_BroadcastHealthChanged();
	UFUNCTION(Client, Reliable)
	void Client_BroadcastOwnTeamLivesChanged();
	UFUNCTION(Client, Reliable)
	void Client_BroadcastEnemyTeamLivesChanged();

	UFUNCTION()
	void ShootingStarted();
	UFUNCTION()
	void ShootingStopped();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void UseAbility();

	UFUNCTION()
	void OnRepCurrentHealth();
	UFUNCTION()
	void OnRepOwnTeamLives();
	UFUNCTION()
	void OnRepEnemyTeamLives();
	
	class UChampionAbilityComponent* AbilityComponent = nullptr;
	class UChampionAnimHandlerComp* AnimHandler = nullptr;
	class UChampionAudioComponent* ChampionAudioComponent = nullptr;
	class ABaseWeapon* Weapon = nullptr;

	void SetCurrentHealth(float newHealth);
	void SetOwnTeamLives(int32 newlives);
	void SetEnemyTeamLives(int32 newlives);

	UPROPERTY(ReplicatedUsing = OnRepCurrentHealth)
	float CurrentHealth = 0.f;

	const float MAX_HEALTH = 100.f;

	UPROPERTY(ReplicatedUsing= OnRepOwnTeamLives)
	int32 OwnTeamLives = 0;
	UPROPERTY(ReplicatedUsing = OnRepEnemyTeamLives)
	int32 EnemyTeamLives = 0;

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
	FORCEINLINE int32 GetOwnTeamLives() const { return OwnTeamLives; }
	FORCEINLINE int32 GetEnemyTeamLives() const { return EnemyTeamLives; }
	FORCEINLINE const ABaseWeapon* GetWeapon() const { return Weapon; }
	FORCEINLINE ABaseWeapon* AccessWeapon() { return Weapon; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UChampionAnimHandlerComp* AccessAnimationHandler() { return AnimHandler; }

	FHealthDelegate OnHealthChangedEvent;
	FChampionDelegate OnDamagedEvent;
	FChampionDelegate OnChampionDeathEvent;
	FChampionDelegate OnAbilityUsedEvent;
	FChampionDelegate OnWeaponAquiredEvent;
	FTeamLivesDelegate OnOwnTeamLivesChangedEvent;
	FTeamLivesDelegate OnEnemyTeamLivesChangedEvent;

};
