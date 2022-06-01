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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChangedEvent;

	//FOnLivesChanged OnLivesTotalChangedEvent;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActorDie OnActorDieEvent;
	
protected:

	//Current Health
	UPROPERTY(Replicated)
	float HealthAmount;
		
	//MAxHealth
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealthAmount;

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	//Util function to restart Health to max health
	void ResetHealthToMax();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(const float Damage);

	// Getters && Setters
	//returns current health
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	
	//returns current health in percentage form to use it in HUD
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthPercentage() const;

	//Sets Health to an amount, clamping between 0 and MaxHealth
	void SetHealth(const float NewHealth);

};

