// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UT_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, OldHealth, float, NewHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLivesChanged, uint8, OldLives, uint8, NewLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDie, AActor*, ActorToDie);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerRespawn, AActor*, ActorToDie);

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
	
	//FOnOwnerRespawn OnOwnerRespawnEvent;

protected:

	//Current Health
	UPROPERTY(ReplicatedUsing = OnRep_HealthAmount)
	float HealthAmount;

	////Current Health
	//UPROPERTY(Replicated)
	//uint8 CurrentLives;
	
	//MAxHealth
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealthAmount;

	////Starting Lives
	//UPROPERTY(EditDefaultsOnly, Category = "Stats")
	//uint8 StartingLives;

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_HealthAmount();
	
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

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentLive() const;

	//Sets Health to an amount, clamping between 0 and MaxHealth
	void SetHealth(const float NewHealth);

	//Sets Current Lives to new value
	void SetLives(const uint8 NewLive);

};

