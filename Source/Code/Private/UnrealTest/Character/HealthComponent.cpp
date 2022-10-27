// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

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
		OnRep_CurrentHealth();
		UE_LOG(LogTemp, Warning, TEXT("Applied Damage"));
		if (OnDamaged.IsBound()) 
		{
			OnDamaged.Broadcast();
		}

		if (OnHealthChanged.IsBound()) 
		{
			OnHealthChanged.Broadcast();
		}

		if (CurrentHealth <= 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("EnemyKilled"));
			Destroy();
		}
	}
}

bool UHealthComponent::CanReceiveDamage()
{
	return true;
}

void UHealthComponent::Destroy()
{
	if (OnDestroyed.IsBound())
	{
		OnDestroyed.Broadcast();
	}
}

//------- Replication --------------------------------------------------------------------------------------------
void UHealthComponent::OnRep_CurrentHealth()
{
}

void UHealthComponent::OnRep_MaxHealth()
{
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_WITH_PARAMS(UHealthComponent, MaxHealth, COND_InitialOnly);
	DOREPLIFETIME_WITH_PARAMS(UHealthComponent, CurrentHealth, COND_InitialOrOwner);
}

//void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

