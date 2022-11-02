// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAbilityComponent.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/AbilityEffect.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

UChampionAbilityComponent::UChampionAbilityComponent()
{
	Cooldown = 5.f;
	Range = 100.f;
	IsAbilityAvailable = true;

	static ConstructorHelpers::FClassFinder<AAbilityEffect> AbilityEffectFinder(TEXT("/Game/Blueprints/Abilities/BP_ShockAbility"));
	if (AbilityEffectFinder.Succeeded())
	{
		AbilityEffectClass = AbilityEffectFinder.Class;
	}
}

void UChampionAbilityComponent::Multicast_TryUseAbility_Implementation()
{
	if (CanUseAbility())
	{
		AChampionCharacter* champion = Cast<AChampionCharacter>(GetOwner());
		if (champion != nullptr) 
		{ 
			UCameraComponent* camera = champion->GetFollowCamera();
			if (camera != nullptr) 
			{ 
				const FVector cameraLocation = camera->GetComponentLocation();
				const FVector spawnLocation = cameraLocation + (camera->GetForwardVector() * Range);
				const FRotator spawnRotation(0.f, 0.f, 0.f);
				const FActorSpawnParameters SpawnInfo;
				AAbilityEffect* spawnedEffect = GetWorld()->SpawnActor<AAbilityEffect>(AbilityEffectClass, spawnLocation, spawnRotation, SpawnInfo);
				spawnedEffect->SetAbilityCreator(GetOwner());
				spawnedEffect->TriggerLifeTimeTimer();

				IsAbilityAvailable = false;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_CooldownTimer, this, &UChampionAbilityComponent::HandleCooldownEnd, Cooldown);

				if (OnAbilityTriggered.IsBound())
				{
					OnAbilityTriggered.Broadcast();
				}
			}
		}		
	}
}

void UChampionAbilityComponent::HandleCooldownEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CooldownTimer);
	IsAbilityAvailable = true;
}


bool UChampionAbilityComponent::CanUseAbility()
{
	return IsAbilityAvailable;
}

void UChampionAbilityComponent::Multicast_ResetAbility_Implementation()
{
	// TODO: Reset any ability stuff in progress.
}
