// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "UnrealTest/Character/ChampionAudioComponent.h"
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Weapons/WeaponAudioComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

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
	
	//UE_LOG(LogTemp, Warning, TEXT("Champion Begin Play!"));
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
	TArray<AActor*> allChildren;
	GetAllChildActors(allChildren, true);
	for (int i = 0; i < allChildren.Num(); i++)
	{
		ABaseWeapon* tempWeapon = Cast<ABaseWeapon>(allChildren[i]);
		if (tempWeapon != nullptr)
		{
			Weapon = tempWeapon;
			Weapon->SetWeaponOwner(this);
			AnimHandler->BindToWeaponEvents(tempWeapon);
			
			if (OnWeaponAquiredEvent.IsBound())
			{
				OnWeaponAquiredEvent.Broadcast();
			}
			break;
		}
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
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Started"));

	if (Weapon != nullptr)
	{
		Weapon->TryUseWeapon();
	}
}

void AChampionCharacter::ShootingStopped()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Stopped"));	
	if (Weapon != nullptr)
	{
		Weapon->StopUsingWeapon();
	}
}

void AChampionCharacter::Reload()
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

		FString msg = FString::Printf(TEXT("[%s] Current Health: %f"), GetOwner() != nullptr ? *GetOwner()->GetName() : *GetName(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *msg);
		
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
	UE_LOG(LogTemp, Warning, TEXT("%s was killed!"), *GetOwner()->GetName());
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


