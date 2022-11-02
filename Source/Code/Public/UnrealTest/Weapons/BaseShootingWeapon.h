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
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponReserveAmmoSignature, int32);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FWeaponClipAmmoSignature, int32, int32);

public:
	ABaseShootingWeapon(const FObjectInitializer& ObjectInitializer);

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

	FWeaponReserveAmmoSignature OnReserveAmmoChangedEvent;
	FWeaponClipAmmoSignature OnClipAmmoChangedEvent;
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
	void Client_BroadcastClipAmmoChanged();
	UFUNCTION(Client, Reliable)
	void Client_BroadcastReserveAmmoChanged();

	UFUNCTION()
	void OnRepCurrentClipAmmo();
	UFUNCTION()
	void OnRepCurrentReserveAmmo();

	void TryApplyDamageToActor(AActor* actor);

	void SetWeaponState(ShootingWeaponStateEnum newState);

	void SetCurrentClipAmmo(int32 clipAmmo);
	void SetCurrentReserveAmmo(int32 reserveAmmo);

	ShootingWeaponStateEnum WeaponState = ShootingWeaponStateEnum::None;

	UPROPERTY(ReplicatedUsing = OnRepCurrentClipAmmo)
	int32 CurrentClipAmmo = 0;
	UPROPERTY(ReplicatedUsing = OnRepCurrentReserveAmmo)
	int32 CurrentReserveAmmo = 0;
	
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
