// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Components/UT_HealthComponent.h"

// Sets default values for this component's properties
UUT_HealthComponent::UUT_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealthAmount = 100.0f;

	//SetHealth to MaxAmount
	ResetHealthToMax();
}


// Called when the game starts
void UUT_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UUT_HealthComponent::ResetHealthToMax()
{
	SetHealth(MaxHealthAmount);
}

void UUT_HealthComponent::ApplyDamage(float Damage)
{
	SetHealth(HealthAmount - Damage);
}

float UUT_HealthComponent::GetCurrentHealth() const
{
	return HealthAmount;
}

float UUT_HealthComponent::GetCurrentHealthPercentage() const
{
	return (HealthAmount/MaxHealthAmount) * 100.f;
}

void UUT_HealthComponent::SetHealth(const float NewHealth)
{
	OnHealthChangedEvent.Broadcast(HealthAmount, NewHealth);
	HealthAmount = FMath::Clamp(NewHealth, 0.f, MaxHealthAmount);

	//DieEvent
	if (HealthAmount <= 0)
	{
		if(GetOwner())
			OnActorDieEvent.Broadcast(GetOwner());
	}
}

