// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "UnrealTest/Interfaces/IDamageable.h"
#include "AbilityEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AAbilityEffect : public ATriggerSphere
{
	GENERATED_BODY()
	
public:
	AAbilityEffect();

	void SetAbilityCreator(AActor* actor);
	void TriggerLifeTimeTimer();

private:
	UFUNCTION()
	void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* overlappedActor, class AActor* otherActor);

	void StartDamageTimer();
	void StopDamageTimer();

	void TryDamageContainedActors();
	void DestroyEffect();

protected:

	UPROPERTY(EditAnywhere)
	float Damage = 0.f;
	UPROPERTY(EditAnywhere)
	float Duration = 0.f;
	UPROPERTY(EditAnywhere)
	int32 DamageRate = 5;

	TArray<class AChampionCharacter*> ContainedChampions;
	AActor* Creator = nullptr;

	FTimerHandle TimerHandle_DamageTimer;
	FTimerHandle TimerHandle_LifeTimeTimer;
};
