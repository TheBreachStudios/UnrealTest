// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealTest/Weapons/ShootingWeaponBase.h"
#include "ShootingWeapon.generated.h"

UCLASS()
class UNREALTEST_API AShootingWeapon : public AActor, public ShootingWeaponBase
{
	GENERATED_BODY()
	
public:	
	AShootingWeapon();

protected:
	virtual void BeginPlay() override;	

	virtual void UseWeapon() override;
	virtual void Reload() override;
	virtual void ResetClipAmmo() override;

public:	
	//virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	int MaxReserveAmmo = 0;

	UPROPERTY(EditAnywhere)
	int MaxClipAmmo = 0;

	int CurrentClipAmmo = 0;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.f;

	UPROPERTY(EditAnywhere)
	float ReloadingTime = 0.f;

};
