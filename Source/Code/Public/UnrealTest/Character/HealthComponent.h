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

	DECLARE_MULTICAST_DELEGATE_TwoParams(FHealthChangedSignature, float, float);
	DECLARE_MULTICAST_DELEGATE(FHealthSignature);

public:	
	UHealthComponent();

	FORCEINLINE float GetMaxHealth() const { return MAX_HEALTH; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION()
	void ResetCurrentHealth();

	virtual bool IsSupportedForNetworking() const override { return true; }

protected:
			
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastHealthChanged();

public:	
	virtual void ApplyDamage(float damage) override;
	virtual bool CanReceiveDamage() override;
	virtual void Destroy() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FHealthChangedSignature OnHealthChangedEvent;
	FHealthSignature OnDamagedEvent;
	FHealthSignature OnHealthEmptyEvent;

protected:
	UPROPERTY(Replicated)
	float CurrentHealth = 0.f;

	const float MAX_HEALTH = 100.f;
};
