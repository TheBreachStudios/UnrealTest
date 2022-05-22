// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Components/HealthComponent.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// GameProject
#include "UnrealTest/Game/UnrealTestGameMode.h"

#pragma region Initialization
// Setup replicated properties.
void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
#pragma endregion Initialization

#pragma region Override
// Called when the game starts.
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Auto bind self to owner's damge events
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}
#pragma endregion Override

#pragma region Functions
// Response for owner's on any damage event.
void UHealthComponent::TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {
	if (GetOwner()->GetLocalRole() >= ROLE_Authority) {
		RemoveHealth(Damage);
	}
}

// Finds and set game mode.
AUnrealTestGameMode* UHealthComponent::FindGameMode() {
	if (!GameModeRef) {
		GameModeRef = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	return GameModeRef;
}

// Initialize health values.
void UHealthComponent::InitializeHealth(float NewHealthValue)
{
	MaxHealth = NewHealthValue;
	CurrentHealth = MaxHealth;
}

// Adds HealthChange to CurrentHealth (to a maximum of MaxHealth).
void UHealthComponent::AddHealth(float HealthChange)
{
	// Checks that HealthChange is positive and is not already at MaxHealth
	if (HealthChange > 0.f && CurrentHealth < MaxHealth) {
		CurrentHealth = FMath::Clamp(CurrentHealth + HealthChange, 0.f, MaxHealth);
	}
}

// Removes HealthChange to CurrentHealth (to a minimum of 0.f).
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

// Get CurrentHealth.
float UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

// Get CurrentHealth normalized.
float UHealthComponent::GetNormalizedHealth()
{
	return CurrentHealth / MaxHealth;
}
#pragma endregion Functions