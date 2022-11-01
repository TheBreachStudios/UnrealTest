// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "BaseShootingWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API ABaseShootingWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FWeaponAmmoSignature, int32, int32, int32);

public:
	ABaseShootingWeapon();

	enum ShootingWeaponStateEnum 
	{
		None,
		Ready,
		Shooting,
		Reloading
	};

	virtual void Reload();
	virtual void TryStartReload();
	bool CanReload();
	virtual void TryUseWeapon() override;
	virtual void ResetWeapon() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FWeaponAmmoSignature OnAmmoChangedEvent;
	FWeaponSignature OnStartedReloadingEvent;
	FWeaponSignature OnEndedReloadingEvent;
	FWeaponSignature OnStartedShootingEvent;
	FWeaponSignature OnEndedShootingEvent;

	FORCEINLINE const int32 GetCurrentClipAmmo() const { return CurrentClipAmmo; }
	FORCEINLINE const int32 GetMaxClipAmmo() const { return MaxClipAmmo; }
	FORCEINLINE const int32 GetCurrentReserveAmmo() const { return CurrentReserveAmmo; }

protected:
	virtual void BeginPlay() override;
	virtual bool CanUseWeapon() override;

	UFUNCTION()
	void HandleEndReloadTime();

	UFUNCTION(Server, Reliable)
	void Server_TraceHitscanShot();

	UFUNCTION(Client, Reliable)
	void Client_BroadcastAmmoChanged();

	void TryApplyDamageToActor(AActor* actor);

	void SetWeaponState(ShootingWeaponStateEnum newState);

	void SetCurrentAmmo(int32 clipAmmo, int32 reserveAmmo);

	ShootingWeaponStateEnum WeaponState = ShootingWeaponStateEnum::None;

	UPROPERTY(Replicated)
	int32 CurrentClipAmmo;
	UPROPERTY(Replicated)
	int32 CurrentReserveAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 MaxClipAmmo;
	UPROPERTY(EditAnywhere)
	int32 MaxReserveAmmo;
	UPROPERTY(EditAnywhere)
	int32 AmmoUsedPerShot;
	UPROPERTY(EditAnywhere)
	float ReloadTime;

	FTimerHandle TimerHandle_ReloadTimer;
};
