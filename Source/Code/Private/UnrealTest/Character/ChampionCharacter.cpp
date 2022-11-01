// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "UnrealTest/Character/ChampionAudioComponent.h"
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Weapons/WeaponAudioComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Components/ChildActorComponent.h"

AChampionCharacter::AChampionCharacter()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	ResetCurrentHealth();
	AnimHandler = CreateDefaultSubobject<UChampionAnimHandlerComp>(TEXT("ChampionAnimationHandlerComponent"));
	AudioComponent = CreateDefaultSubobject<UChampionAudioComponent>(TEXT("ChampionAudioComponent"));
}

void AChampionCharacter::Multicast_ResetChampionCharacter_Implementation()
{
	ResetCurrentHealth();
	AnimHandler->Multicast_ResetAnimation();
	AudioComponent->Multicast_ResetAudio();
	if (Weapon != nullptr)
	{
		Weapon->ResetWeapon();
	}
}

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (Weapon == nullptr)
	{
		TryFindWeapon();
	}
	
}

void AChampionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ShootBinding(PlayerInputComponent);
	ReloadBinding(PlayerInputComponent);
}

void AChampionCharacter::ShootBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AChampionCharacter::ShootingStarted);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AChampionCharacter::ShootingStopped);
}

void AChampionCharacter::ReloadBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AChampionCharacter::Reload);
}

void AChampionCharacter::HandleDeath()
{
	FString msg = FString::Printf(TEXT("[Client] %s Died"), *GetName());
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, *msg);

	if (OnChampionDeathEvent.IsBound())
	{
		OnChampionDeathEvent.Broadcast();
	}

	AudioComponent->Multicast_PlayDeathSFX_Implementation();
}

void AChampionCharacter::TryFindWeapon()
{
	UChildActorComponent* childComponent = FindComponentByClass<UChildActorComponent>();
	if (childComponent != nullptr)
	{
		ABaseWeapon* childWeapon = Cast<ABaseWeapon>(childComponent->GetChildActor());
		if (childWeapon != nullptr)
		{
			Weapon = childWeapon;
			Weapon->SetWeaponOwner(this);			
			AnimHandler->BindToWeaponEvents(childWeapon);

			if (OnWeaponAquiredEvent.IsBound())
			{
				OnWeaponAquiredEvent.Broadcast();
			}

			if (GetLocalRole() < ENetRole::ROLE_Authority)
			{
				UE_LOG(LogTemp, Warning, TEXT("[Client][%s] Found weapon."), *GetName());
				UE_LOG(LogTemp, Warning, TEXT("[Client][%s] Owner is %s."), *GetName(), childWeapon->GetOwner() != nullptr ? *childWeapon->GetOwner()->GetName() : TEXT("NULL"));
				UE_LOG(LogTemp, Warning, TEXT("[Client][%s] NetOwner is %s."), *GetName(), childWeapon->GetNetOwner() != nullptr ? *childWeapon->GetNetOwner()->GetName() : TEXT("NULL"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[Server][%s] Found weapon"), *GetName());
				UE_LOG(LogTemp, Warning, TEXT("[Server][%s] Owner is %s."), *GetName(), childWeapon->GetOwner() != nullptr ? *childWeapon->GetOwner()->GetName() : TEXT("NULL"));
				UE_LOG(LogTemp, Warning, TEXT("[Server][%s] NetOwner is %s."), *GetName(), childWeapon->GetNetOwner() != nullptr ? *childWeapon->GetNetOwner()->GetName() : TEXT("NULL"));
			}
		}
	}
}

void AChampionCharacter::HandleOnWeaponInitialized()
{
	if (Weapon == nullptr)
	{
		TryFindWeapon();
	}
}

void AChampionCharacter::Server_ReloadWeapon_Implementation()
{
	if (Weapon != nullptr)
	{
		ABaseShootingWeapon* shootingWeapon = Cast<ABaseShootingWeapon>(Weapon);
		if (shootingWeapon != nullptr)
		{
			shootingWeapon->TryStartReload();
		}
	}
}

void AChampionCharacter::Server_UseWeapon_Implementation()
{
	if (Weapon != nullptr)
	{
		Weapon->TryUseWeapon();
	}
}

void AChampionCharacter::ResetCurrentHealth()
{
	CurrentHealth = MAX_HEALTH;
	OnRepCurrentHealth();
}

void AChampionCharacter::Client_BroadcastHealthChanged_Implementation()
{
	if (OnHealthChangedEvent.IsBound())
	{
		OnHealthChangedEvent.Broadcast(CurrentHealth, MAX_HEALTH);
	}
}

void AChampionCharacter::ShootingStarted()
{
	if (Weapon != nullptr)
	{
		Server_UseWeapon();		
	}
}

void AChampionCharacter::ShootingStopped()
{
	if (Weapon != nullptr)
	{
		Weapon->StopUsingWeapon();
	}
}

void AChampionCharacter::Reload()
{
	if (Weapon != nullptr)
	{
		Server_ReloadWeapon();
	}
}

void AChampionCharacter::OnRepCurrentHealth()
{
	Client_BroadcastHealthChanged();
}

void AChampionCharacter::ApplyDamage(float damage)
{
	if (CanReceiveDamage() && damage > 0.f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - damage, 0.f, MAX_HEALTH);
		OnRepCurrentHealth();

		//if (GetLocalRole() < ENetRole::ROLE_Authority) 
		//{
		//	FString msg = FString::Printf(TEXT("[Client][%s] Current Health: %f"), GetOwner() != nullptr ? *GetOwner()->GetName() : *GetName(), CurrentHealth);
		//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *msg);
		//}
		//else 
		//{
		//	FString msg = FString::Printf(TEXT("[Server][%s] Current Health: %f"), GetOwner() != nullptr ? *GetOwner()->GetName() : *GetName(), CurrentHealth);
		//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *msg);
		//}
		
		if (OnDamagedEvent.IsBound())
		{
			OnDamagedEvent.Broadcast();
		}

		if (CurrentHealth <= 0.f)
		{
			Destroy();
		}
	}
}

bool AChampionCharacter::CanReceiveDamage()
{
	return CurrentHealth > 0.f;
}

void AChampionCharacter::Destroy()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s was killed!"), *GetOwner()->GetName());
	if (OnChampionDeathEvent.IsBound())
	{
		OnChampionDeathEvent.Broadcast();
	}
}

void AChampionCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChampionCharacter, CurrentHealth);
}


