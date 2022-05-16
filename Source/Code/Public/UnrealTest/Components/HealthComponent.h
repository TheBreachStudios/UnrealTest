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
	// Sets default values for this component's properties
	UHealthComponent();
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
	// Called when the game starts
	virtual void BeginPlay() override;
#pragma endregion Override

#pragma region Functions
protected:
	UFUNCTION()
	void TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	AUnrealTestGameMode* FindGameMode();

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeHealth(float NewHealthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float HealthChange);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void RemoveHealth(float HealthChange);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetNormalizedHealth();
#pragma endregion Functions

};
