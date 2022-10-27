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

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;
	void ResetHealth();
	
	virtual bool CanReceiveDamage() override;
	virtual void Destroy() override;

public:	
	virtual void ApplyDamage(float damage) override;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FHealthSignature OnHealthChanged;
	FHealthSignature OnDamaged;
	FHealthSignature OnDestroyed;

private:
	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_MaxHealth();

protected:
	UPROPERTY(Replicated, EditAnywhere)
	float MaxHealth = 0.f;
	
	UPROPERTY(Replicated)
	float CurrentHealth = 0.f;
};
