// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChampionAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UChampionAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FAbilityDelegate);

public:	
	UChampionAbilityComponent();	

protected:
	void HandleCooldownEnd();

	UPROPERTY(EditAnywhere)
	float Cooldown = 0.f;
	UPROPERTY(EditAnywhere)
	float Range = 0.f;

	bool IsAbilityAvailable = true;

	FTimerHandle TimerHandle_CooldownTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityEffect> AbilityEffectClass = nullptr;

public:	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryUseAbility();
	bool CanUseAbility();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetAbility();
		
	FAbilityDelegate OnAbilityTriggered;
};
