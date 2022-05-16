// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Components/HealthComponent.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// GameProject
#include "UnrealTest/Game/UnrealTestGameMode.h"

#pragma region Initialization

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
#pragma endregion Initialization

#pragma region Override
// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	
}
#pragma endregion Override

#pragma region Functions

void UHealthComponent::TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {
	if (GetOwner()->GetLocalRole() >= ROLE_Authority) {
		RemoveHealth(Damage);
	}
}

void UHealthComponent::InitializeHealth(float NewHealthValue)
{
	MaxHealth = NewHealthValue;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::AddHealth(float HealthChange)
{
	// Checks that HealthChange is positive and is not already at MaxHealth
	if (HealthChange > 0.f && CurrentHealth < MaxHealth) {
		CurrentHealth = FMath::Clamp(CurrentHealth + HealthChange, 0.f, MaxHealth);
	}
}

void UHealthComponent::RemoveHealth(float HealthChange)
{
	// Check HealthChange is positive and is not dead already
	if (HealthChange > 0.f && CurrentHealth > 0.f) {
		CurrentHealth = FMath::Clamp(CurrentHealth - HealthChange, 0.f, MaxHealth);
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Emerald, FString::Printf(TEXT("[LocalRole]: Damaged! %f/%f"), CurrentHealth, MaxHealth));

		// If owner would die with that amount of damage warn the GameMode about the dead
		if (CurrentHealth <= 0.f) {
			AActor* DamagedActor = GetOwner();

			GEngine->AddOnScreenDebugMessage(1, 2, FColor::Emerald, FString::Printf(TEXT("[LocalRole]: Die!"), CurrentHealth, MaxHealth));
			FindGameMode()->ActorDied(DamagedActor);
		}
	}
}

float UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float UHealthComponent::GetNormalizedHealth()
{
	return CurrentHealth / MaxHealth;
}

AUnrealTestGameMode* UHealthComponent::FindGameMode() {
	if (!GameModeRef) {
		GameModeRef = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	return GameModeRef;
}

#pragma endregion Functions