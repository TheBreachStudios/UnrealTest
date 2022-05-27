// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UT_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, OldHealth, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDie, AActor*, ActorToDie);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UUT_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUT_HealthComponent();

	FOnHealthChanged OnHealthChangedEvent;
	FOnActorDie OnActorDieEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Current Health
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float HealthAmount;
	
	//MAxHealth
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealthAmount;

public:	
	//Util function to restart Health to max health
	void ResetHealthToMax();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(const float Damage);
	// Getters && Setters
	//returns current health
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	
	//returns current health in percentage form
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthPercentage() const;

	//Sets Health to an amount, clamping between 0 and MaxHealth
	void SetHealth(const float NewHealth);
};
