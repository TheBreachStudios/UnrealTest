// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "UnrealTest/Character/ChampionAudioComponent.h"
#include "UnrealTest/Character/ChampionAbilityComponent.h"
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Weapons/WeaponAudioComponent.h"
#include "UnrealTest/Game/EliminationGameState.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Components/ChildActorComponent.h"

AChampionCharacter::AChampionCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;

	SetCurrentHealth(MAX_HEALTH);
	
	AnimHandler = ObjectInitializer.CreateDefaultSubobject<UChampionAnimHandlerComp>(this, TEXT("ChampionAnimHandler"));
	ChampionAudioComponent = ObjectInitializer.CreateDefaultSubobject<UChampionAudioComponent>(this, TEXT("ChampionAudioComponent"));
	AbilityComponent = ObjectInitializer.CreateDefaultSubobject<UChampionAbilityComponent>(this, TEXT("ChampionAbilityComponent"));
}

void AChampionCharacter::Multicast_ResetChampionCharacter_Implementation()
{
	SetCurrentHealth(MAX_HEALTH);
	if (AnimHandler != nullptr)
	{
		AnimHandler->Multicast_ResetAnimation();
	}

	if (ChampionAudioComponent != nullptr)
	{
		ChampionAudioComponent->Multicast_ResetAudio();
	}

	if (Weapon != nullptr)
	{
		Weapon->ResetWeapon();
	}

	if (AbilityComponent != nullptr)
	{
		AbilityComponent->Multicast_ResetAbility();
	}
}

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindToGameStateEvents();
	BindToAbilityEvents();

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
	AbilityBinding(PlayerInputComponent);
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

void AChampionCharacter::AbilityBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("UseAbility", IE_Pressed, this, &AChampionCharacter::UseAbility);
}

void AChampionCharacter::HandleDeath()
{
	FString msg = FString::Printf(TEXT("[Client] %s Died"), *GetName());
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, *msg);

	if (OnChampionDeathEvent.IsBound())
	{
		OnChampionDeathEvent.Broadcast();
	}

	if (ChampionAudioComponent != nullptr)
	{
		ChampionAudioComponent->Multicast_PlayDeathSFX_Implementation();
	}
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
			if (AnimHandler != nullptr)
			{
				AnimHandler->BindToWeaponEvents(childWeapon);
			}

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

void AChampionCharacter::BindToGameStateEvents()
{
	AEliminationGameState* gameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AEliminationGameState>() : nullptr;
	if (gameState != nullptr)
	{
		gameState->OnTeamLivesChanged.AddUObject(this, &AChampionCharacter::HandleOnTeamLivesChanged);
		SetOwnTeamLives(gameState->GetMaxLives());
		SetEnemyTeamLives(gameState->GetMaxLives());
	}
}

void AChampionCharacter::BindToAbilityEvents()
{
	if (AbilityComponent != nullptr)
	{
		AbilityComponent->OnAbilityTriggered.AddUObject(this, &AChampionCharacter::HandleOnAbilityTriggered);
	}
}

void AChampionCharacter::HandleOnAbilityTriggered()
{
	if (OnAbilityUsedEvent.IsBound())
	{
		OnAbilityUsedEvent.Broadcast();
	}
}

void AChampionCharacter::HandleOnTeamLivesChanged(int32 teamID, int32 lives)
{
	AEliminationGameState* gameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AEliminationGameState>() : nullptr;
	if (gameState != nullptr)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			int32 tempOwnTeamLives = 0;
			int32 tempEnemyTeamLives = 0;
			gameState->GetAllTeamLives(playerController, tempOwnTeamLives, tempEnemyTeamLives);

			SetOwnTeamLives(tempOwnTeamLives);
			SetEnemyTeamLives(tempEnemyTeamLives);
		}
	}
}

void AChampionCharacter::Server_UseChampionAbility_Implementation()
{
	if (AbilityComponent != nullptr)
	{
		AbilityComponent->Multicast_TryUseAbility();
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

void AChampionCharacter::Client_BroadcastHealthChanged_Implementation()
{
	if (OnHealthChangedEvent.IsBound())
	{
		OnHealthChangedEvent.Broadcast(CurrentHealth, MAX_HEALTH);
	}
}

void AChampionCharacter::Client_BroadcastOwnTeamLivesChanged_Implementation()
{
	if (OnOwnTeamLivesChangedEvent.IsBound())
	{
		OnOwnTeamLivesChangedEvent.Broadcast(OwnTeamLives);
	}
}

void AChampionCharacter::Client_BroadcastEnemyTeamLivesChanged_Implementation()
{
	if (OnEnemyTeamLivesChangedEvent.IsBound())
	{
		OnEnemyTeamLivesChangedEvent.Broadcast(EnemyTeamLives);
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

void AChampionCharacter::UseAbility()
{
	if (AbilityComponent != nullptr)
	{
		Server_UseChampionAbility();
	}
}

void AChampionCharacter::OnRepCurrentHealth()
{
	Client_BroadcastHealthChanged();
}

void AChampionCharacter::OnRepOwnTeamLives()
{
	Client_BroadcastOwnTeamLivesChanged();
}

void AChampionCharacter::OnRepEnemyTeamLives()
{
	Client_BroadcastEnemyTeamLivesChanged();
}

void AChampionCharacter::SetCurrentHealth(float newHealth)
{
	CurrentHealth = newHealth;
	OnRepCurrentHealth();
}

void AChampionCharacter::SetOwnTeamLives(int32 newlives)
{
	OwnTeamLives = newlives;
	OnRepOwnTeamLives();
}

void AChampionCharacter::SetEnemyTeamLives(int32 newlives)
{
	EnemyTeamLives = newlives;
	OnRepEnemyTeamLives();
}

void AChampionCharacter::ApplyDamage(float damage)
{
	if (CanReceiveDamage() && damage > 0.f)
	{
		SetCurrentHealth(FMath::Clamp(CurrentHealth - damage, 0.f, MAX_HEALTH));

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
	DOREPLIFETIME(AChampionCharacter, OwnTeamLives);
	DOREPLIFETIME(AChampionCharacter, EnemyTeamLives);
}


