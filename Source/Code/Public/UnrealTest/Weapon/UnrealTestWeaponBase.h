// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealTestWeaponBase.generated.h"

/**
* Abstract class representing a weapon
*/

UCLASS(Abstract)
class UNREALTEST_API AUnrealTestWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnrealTestWeaponBase();

public:
	/**
	* Get this weapon's animation class
	*/
	UFUNCTION(BlueprintCallable)
	UClass* GetWeaponAnimClass() { return WeaponAnimClass; }

	/**
	* Get this weapon's animation montage
	*/
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetWeaponAnimMontage() { return WeaponUseAnimMontage; }

	UFUNCTION(BlueprintCallable)
	float GetDamage() { return Damage; }

	virtual void OnAttack() {}

protected:
	/**
	* Gun mesh
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPWeaponMesh;

	/**
	* Anim class to use when this weapon is equipped
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UClass* WeaponAnimClass;

	/**
	* AnimMontage to play each time the weapon is used
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* WeaponUseAnimMontage;

	/**
	* This weapon's damage
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Damage;
};
