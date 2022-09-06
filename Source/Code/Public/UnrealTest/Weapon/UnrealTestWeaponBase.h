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

	/** Get this weapon's mesh */
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	/** Get this weapon's damage */
	UFUNCTION(BlueprintCallable)
	float GetDamage() { return Damage; }

	virtual void OnAttack() {}

protected:
	/** Weapon mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* WeaponMesh;

	/** This weapon's damage */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Damage;
};
