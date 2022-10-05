// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/UnrealTestGameplayAbility.h"
#include "UTFireGameplayAbility.generated.h"

class AUTFireActor;

/**
 * Base Ability for shooting
 */
UCLASS()
class UNREALTEST_API UUTFireGameplayAbility : public UUnrealTestGameplayAbility
{
	GENERATED_BODY()
public:
	UUTFireGameplayAbility();

		/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								const FGameplayAbilityActorInfo* ActorInfo, 
								const FGameplayAbilityActivationInfo ActivationInfo, 
								const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AUTFireActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

};
