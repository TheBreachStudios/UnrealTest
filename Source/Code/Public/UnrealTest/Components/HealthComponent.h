// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Game Project
#include "HealthComponent.generated.h"

// Definitions
class AUnrealTestGameMode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Initialization
// Initialization
public:
	// Setup replicated properties.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion Initialization

#pragma region Configuration
// Configuration
private:
	// Default maximum health 
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth = 100.f;
#pragma endregion Configuration

#pragma region Variables
// Variables
private:
	// Current health
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float CurrentHealth = 0.f;

	AUnrealTestGameMode* GameModeRef = nullptr;
#pragma region Variables

#pragma region Override
// Override
protected:
	// Called when the game starts.
	virtual void BeginPlay() override;
#pragma endregion Override

#pragma region Functions
protected:
	// Response for owner's on any damage event.
	UFUNCTION()
	void TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// Finds and set game mode.
	AUnrealTestGameMode* FindGameMode();

public:
	// Initializes health values.
	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeHealth(float NewHealthValue);

	// Adds HealthChange to CurrentHealth (to a maximum of MaxHealth).
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float HealthChange);

	// Removes HealthChange to CurrentHealth (to a minimum of 0.f).
	UFUNCTION(BlueprintCallable, Category = "Health")
	void RemoveHealth(float HealthChange);

	// Get CurrentHealth.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth();

	// Get CurrentHealth normalized.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetNormalizedHealth();
#pragma endregion Functions

};
