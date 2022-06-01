// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Components/UT_HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UUT_HealthComponent::UUT_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	//Replicated by default in case the owner doesnt replicates it
	SetIsReplicatedByDefault(true);
	MaxHealthAmount = 100.0f;

	//SetHealth to MaxAmount
	ResetHealthToMax();
}

void UUT_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UUT_HealthComponent, HealthAmount);
}

void UUT_HealthComponent::ResetHealthToMax()
{
	SetHealth(MaxHealthAmount);
}

void UUT_HealthComponent::TakeDamage(float Damage)
{
	SetHealth(HealthAmount - Damage);
}

float UUT_HealthComponent::GetCurrentHealth() const
{
	return HealthAmount;
}

float UUT_HealthComponent::GetCurrentHealthPercentage() const
{
	return (HealthAmount/MaxHealthAmount);
}

void UUT_HealthComponent::SetHealth(const float NewHealth)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		OnHealthChangedEvent.Broadcast(HealthAmount, NewHealth);
		HealthAmount = FMath::Clamp(NewHealth, 0.f, MaxHealthAmount);

		//DieEvent
		if (HealthAmount <= 0)
		{
			if (GetOwner())
			{
				OnActorDieEvent.Broadcast(GetOwner());
			}
		}
	}
}