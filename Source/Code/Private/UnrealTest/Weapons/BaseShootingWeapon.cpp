// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"


ABaseShootingWeapon::ABaseShootingWeapon()
{
	WeaponState = ShootingWeaponStateEnum::None;

	MaxClipAmmo = 30;
	CurrentClipAmmo = MaxClipAmmo;
	MaxReserveAmmo = 100;
	CurrentReserveAmmo = MaxReserveAmmo;
	AmmoUsedPerShot = 1;
	ReloadTime = 3.3f;
}

bool ABaseShootingWeapon::CanReload()
{
	return CurrentClipAmmo < MaxClipAmmo && WeaponState == ShootingWeaponStateEnum::Ready;
}

void ABaseShootingWeapon::TryStartReload()
{
	if (CanReload())
	{
		SetWeaponState(ShootingWeaponStateEnum::Reloading);
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadTimer, this, &ABaseShootingWeapon::HandleEndReloadTime, ReloadTime);
	}
}

void ABaseShootingWeapon::HandleEndReloadTime()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ReloadTimer);
	Reload();
	SetWeaponState(ShootingWeaponStateEnum::Ready);
}

void ABaseShootingWeapon::Reload()
{
	float neededClipAmmo = MaxClipAmmo - CurrentClipAmmo;
	float addedAmmo = CurrentReserveAmmo >= neededClipAmmo ? neededClipAmmo : CurrentReserveAmmo;
	CurrentClipAmmo += addedAmmo;
	CurrentReserveAmmo -= addedAmmo;
}

void ABaseShootingWeapon::TryUseWeapon()
{
	if (CanUseWeapon())
	{
		Super::TryUseWeapon();
		CurrentClipAmmo = FMath::Clamp(CurrentClipAmmo - AmmoUsedPerShot, 0, MaxClipAmmo);
	}
}

void ABaseShootingWeapon::ResetWeapon()
{
	Super::ResetWeapon();

	SetWeaponState(ShootingWeaponStateEnum::Ready);

	CurrentClipAmmo = MaxClipAmmo;
	CurrentReserveAmmo = MaxReserveAmmo;
}

void ABaseShootingWeapon::BeginPlay()
{
	SetWeaponState(ShootingWeaponStateEnum::Ready);
}

bool ABaseShootingWeapon::CanUseWeapon()
{
	return CurrentClipAmmo > 0 && WeaponState == ShootingWeaponStateEnum::Ready;
}

void ABaseShootingWeapon::Server_TraceHitscanShot_Implementation()
{
	FHitResult hitResult = FHitResult();
	if (OwningPawn == nullptr) { return; }

	// TODO: later change to account for AI weapon users.
	AChampionCharacter* champion = Cast<AChampionCharacter>(OwningPawn);
	if (champion == nullptr) { return; }

	UCameraComponent* camera = champion->GetFollowCamera();
	if (camera == nullptr) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Hitscan Shot!"));
	const FVector cameraLocation = camera->GetComponentLocation();
	// To avoid hitting something behind the weapon.
	const float cameraToCharacterDist = (GetActorLocation() - cameraLocation).Size();
	const FVector startLocation = cameraLocation + (camera->GetForwardVector() * cameraToCharacterDist);
	const FVector endLocation = startLocation + (camera->GetForwardVector() * Range);
	const ECollisionChannel channel = ECollisionChannel::ECC_Pawn;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, channel, traceParams);
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Yellow, true, -1, 0, 10.f);
	//DrawDebugSphere(GetWorld(), startLocation, 10.f, 10.f, FColor::Blue, true, 10.f);
	//DrawDebugSphere(GetWorld(), endLocation, 10.f, 10.f, FColor::Red, true, 10.f);

	if (hitResult.IsValidBlockingHit())
	{
		TryApplyDamageToActor(hitResult.GetActor());
	}
}

void ABaseShootingWeapon::TryApplyDamageToActor(AActor* actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *actor->GetName());
	UHealthComponent* hpComponent = Cast<UHealthComponent>(actor->GetComponentByClass(UHealthComponent::StaticClass()));
	if (hpComponent != nullptr)
	{
		hpComponent->ApplyDamage(Damage);
	}
}

void ABaseShootingWeapon::SetWeaponState(ShootingWeaponStateEnum newState)
{
	if (newState == WeaponState) { return; }

	// Start Events
	if (newState == ShootingWeaponStateEnum::Reloading)
	{
		if (OnStartedReloadingEvent.IsBound()) 
		{
			OnStartedReloadingEvent.Broadcast();
		}
	}
	else if (newState == ShootingWeaponStateEnum::Shooting)
	{
		if (OnStartedShootingEvent.IsBound())
		{
			OnStartedShootingEvent.Broadcast();
		}
	}

	// End Events
	if (WeaponState == ShootingWeaponStateEnum::Reloading)
	{
		if (OnEndedReloadingEvent.IsBound())
		{
			OnEndedReloadingEvent.Broadcast();
		}
	}
	else if (WeaponState == ShootingWeaponStateEnum::Shooting)
	{
		if (OnEndedShootingEvent.IsBound())
		{
			OnEndedShootingEvent.Broadcast();
		}
	}

	WeaponState = newState;
}

void ABaseShootingWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseShootingWeapon, CurrentClipAmmo);
	DOREPLIFETIME(ABaseShootingWeapon, CurrentReserveAmmo);
}

