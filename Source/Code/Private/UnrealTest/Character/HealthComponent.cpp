// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	ResetCurrentHealth();
}

void UHealthComponent::ResetCurrentHealth()
{
	CurrentHealth = MAX_HEALTH;
	OnRep_CurrentHealth();
}

void UHealthComponent::ApplyDamage(float damage)
{
	if (CanReceiveDamage() && damage > 0.f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - damage, 0.f, MAX_HEALTH);
		OnRep_CurrentHealth();

		FString msg = FString::Printf(TEXT("[%s] Current Health: %f"), GetOwner() != nullptr ? *GetOwner()->GetName() : *GetName(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *msg);

		if (OnHealthChangedEvent.IsBound())
		{
			OnHealthChangedEvent.Broadcast(CurrentHealth, MAX_HEALTH);
		}

		if (OnDamagedEvent.IsBound())
		{
			OnDamagedEvent.Broadcast();
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
	if (OnHealthEmptyEvent.IsBound())
	{
		OnHealthEmptyEvent.Broadcast();
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::OnHealthUpdated()
{
	//const FString messageStr = FString::Printf(TEXT("Health: %f"), CurrentHealth);
	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, messageStr);
}

void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthUpdated();
}

