// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Interfaces/IDamageable.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"


ABaseShootingWeapon::ABaseShootingWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	int32 newClipAmmo = CurrentClipAmmo + addedAmmo;
	int32 newReserveAmmo = CurrentReserveAmmo - addedAmmo;
	SetCurrentClipAmmo(newClipAmmo);
	SetCurrentReserveAmmo(newReserveAmmo);
}

void ABaseShootingWeapon::TryUseWeapon()
{
	if (CanUseWeapon())
	{
		Super::TryUseWeapon();
		int32 newClipAmmo = FMath::Clamp(CurrentClipAmmo - AmmoUsedPerShot, 0, MaxClipAmmo);
		SetCurrentClipAmmo(newClipAmmo);
	}
}

void ABaseShootingWeapon::ResetWeapon()
{
	Super::ResetWeapon();

	SetWeaponState(ShootingWeaponStateEnum::Ready);

	SetCurrentClipAmmo(MaxClipAmmo);
	SetCurrentReserveAmmo(MaxReserveAmmo);
}

void ABaseShootingWeapon::BeginPlay()
{	
	Super::BeginPlay();

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

void ABaseShootingWeapon::Client_BroadcastClipAmmoChanged_Implementation()
{
	if (OnClipAmmoChangedEvent.IsBound())
	{
		OnClipAmmoChangedEvent.Broadcast(CurrentClipAmmo, MaxClipAmmo);
	}
}

void ABaseShootingWeapon::Client_BroadcastReserveAmmoChanged_Implementation()
{
	if (OnReserveAmmoChangedEvent.IsBound())
	{
		OnReserveAmmoChangedEvent.Broadcast(CurrentReserveAmmo);
	}
}

void ABaseShootingWeapon::OnRepCurrentClipAmmo()
{
	Client_BroadcastClipAmmoChanged();
}

void ABaseShootingWeapon::OnRepCurrentReserveAmmo()
{
	Client_BroadcastReserveAmmoChanged();
}

void ABaseShootingWeapon::TryApplyDamageToActor(AActor* actor)
{
	if (GetLocalRole() < ENetRole::ROLE_Authority) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[Client][%s] Hit Actor: %s"), *GetName(), *actor->GetName());
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server][%s] Hit Actor: %s"), *GetName(), *actor->GetName());
	}

	// TODO: Later account for non player actors.
	AChampionCharacter* champion = Cast<AChampionCharacter>(actor);
	if (champion != nullptr)
	{
		champion->ApplyDamage(Damage);
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

void ABaseShootingWeapon::SetCurrentClipAmmo(int32 clipAmmo)
{
	CurrentClipAmmo = clipAmmo;
	OnRepCurrentClipAmmo();

	//FString msg = FString::Printf(TEXT("[%s][%s] Current Clip Ammo: %d | Current Reserve Ammo: %d"),
	//	GetLocalRole() < ENetRole::ROLE_Authority ? TEXT("Client") : TEXT("Server"),
	//	GetOwner() != nullptr ? *GetOwner()->GetName() : *GetName(),
	//	CurrentClipAmmo, CurrentReserveAmmo);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, *msg);
}

void ABaseShootingWeapon::SetCurrentReserveAmmo(int32 reserveAmmo)
{
	CurrentReserveAmmo = reserveAmmo;
	OnRepCurrentReserveAmmo();

	//FString msg = FString::Printf(TEXT("[%s][%s] Current Clip Ammo: %d | Current Reserve Ammo: %d"),
	//	GetLocalRole() < ENetRole::ROLE_Authority ? TEXT("Client") : TEXT("Server"),
	//	GetOwner() != nullptr ? *GetOwner()->GetName() : *GetName(),
	//	CurrentClipAmmo, CurrentReserveAmmo);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, *msg);
}

void ABaseShootingWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseShootingWeapon, CurrentClipAmmo);
	DOREPLIFETIME(ABaseShootingWeapon, CurrentReserveAmmo);
}

