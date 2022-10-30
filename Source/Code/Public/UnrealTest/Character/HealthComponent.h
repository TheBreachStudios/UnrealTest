// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnrealTest/Interfaces/IDamageable.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UHealthComponent : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FHealthSignature);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FHealthChangedSignature, float, float);

public:	
	UHealthComponent();

	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

protected:
	void ResetCurrentHealth();
			
	UFUNCTION()
	void OnRep_CurrentHealth();

public:	
	virtual void ApplyDamage(float damage) override;
	virtual bool CanReceiveDamage() override;
	virtual void Destroy() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FHealthChangedSignature OnHealthChangedEvent;
	FHealthSignature OnDamagedEvent;
	FHealthSignature OnHealthEmptyEvent;

	void OnHealthUpdated();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth = 0.f;

	float MaxHealth = 100.f;
};
