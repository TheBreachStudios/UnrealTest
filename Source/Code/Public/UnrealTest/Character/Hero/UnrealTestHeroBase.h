// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTestHeroBase.generated.h"

class AUnrealTestWeaponBase;

/**
 * Abstract base class parent to all heroes. Contains common logic to heroes.
 */
UCLASS(Abstract)
class UNREALTEST_API AUnrealTestHeroBase : public AUnrealTestCharacter
{
	GENERATED_BODY()

public:
	AUnrealTestHeroBase();

	/**
	 * Get the current health of this hero
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	/**
	 * Get the maximum health of this hero
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/**
	 * Get hero's weapon
	 */
	UFUNCTION()
	AUnrealTestWeaponBase* GetWeapon() { return Weapon; }

protected:
	/**
	* Max health of the hero. Initial value would ideally be specified in a data table to make it more accesible to game designers
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100;

	/**
	 * Current health of this hero
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AUnrealTestWeaponBase* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AUnrealTestWeaponBase> DefaultWeaponClass;
};
