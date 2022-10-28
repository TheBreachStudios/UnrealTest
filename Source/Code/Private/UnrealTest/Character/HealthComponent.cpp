// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	MaxHealth = 0.f;
	CurrentHealth = 0.f;
}


void UHealthComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	ResetHealth();	
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyDamage(float damage)
{
	if (CanReceiveDamage() && damage > 0.f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - damage, 0.f, MaxHealth);

		UE_LOG(LogTemp, Warning, TEXT("%s took %f damage!"), *GetOwner()->GetName(), damage);

		if (OnDamaged.IsBound()) 
		{
			OnDamaged.Broadcast();
		}

		if (OnHealthChanged.IsBound()) 
		{
			OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
		}

		if (CurrentHealth <= 0.f)
		{			
			Destroy();
		}
	}
}

bool UHealthComponent::CanReceiveDamage()
{
	return CurrentHealth > 0.f;
}

void UHealthComponent::Destroy()
{
	UE_LOG(LogTemp, Warning, TEXT("%s was killed!"), *GetOwner()->GetName());
	if (OnHealthEmpty.IsBound())
	{
		OnHealthEmpty.Broadcast();
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_WITH_PARAMS(UHealthComponent, MaxHealth, COND_InitialOnly);
	DOREPLIFETIME_WITH_PARAMS(UHealthComponent, CurrentHealth, COND_InitialOrOwner);
}

