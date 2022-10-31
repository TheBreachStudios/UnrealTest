// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class UNREALTEST_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	virtual void TryUseWeapon();
	virtual void StopUsingWeapon();
	virtual void ResetWeapon();
	void SetWeaponOwner(APawn* pawn);	

protected:	
	DECLARE_MULTICAST_DELEGATE(FWeaponSignature);

	virtual bool CanUseWeapon();

	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float Range;
	UPROPERTY(EditAnywhere)
	int32 AttackRate; // Attacks per second

	APawn* OwningPawn = nullptr;

};
