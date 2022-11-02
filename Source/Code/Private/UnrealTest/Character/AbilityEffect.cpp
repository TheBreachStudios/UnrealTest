// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/AbilityEffect.h"
#include "UnrealTest/Character/ChampionCharacter.h"

AAbilityEffect::AAbilityEffect()
{
	Damage = 5.f;
	Duration = 3.5f;
	DamageRate = 5;

	OnActorBeginOverlap.AddDynamic(this, &AAbilityEffect::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AAbilityEffect::OnOverlapEnd);
}

void AAbilityEffect::SetAbilityCreator(AActor* actor)
{
	Creator = actor;
}

void AAbilityEffect::TriggerLifeTimeTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandle_LifeTimeTimer, this, &AAbilityEffect::DestroyEffect, Duration);
}

void AAbilityEffect::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this && (Creator == nullptr || (Creator != nullptr && OtherActor != Creator)))
	{
		AChampionCharacter* champion = Cast<AChampionCharacter>(OtherActor);
		if (champion != nullptr)
		{			
			if (!ContainedChampions.Contains(champion))
			{
				ContainedChampions.Add(champion);
			}

			if (ContainedChampions.Num() == 1)
			{
				TryDamageContainedActors();
				StartDamageTimer();
			}
		}
	}
}

void AAbilityEffect::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this && (Creator == nullptr || (Creator != nullptr && OtherActor != Creator)))
	{
		AChampionCharacter* champion = Cast<AChampionCharacter>(OtherActor);
		if (champion != nullptr)
		{
			if (ContainedChampions.Contains(champion))
			{
				ContainedChampions.Remove(champion);
			}

			if (ContainedChampions.Num() <= 0)
			{
				StopDamageTimer();
			}
		}
	}
}

void AAbilityEffect::StartDamageTimer()
{
	float damageTick = DamageRate > 0 ? 1.f / DamageRate : 1.f;
	GetWorldTimerManager().SetTimer(TimerHandle_DamageTimer, this, &AAbilityEffect::TryDamageContainedActors, damageTick, true);
}

void AAbilityEffect::StopDamageTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DamageTimer);
}

void AAbilityEffect::TryDamageContainedActors()
{
	for (int i = 0; i < ContainedChampions.Num(); i++)
	{
		ContainedChampions[i]->ApplyDamage(Damage);
	}
}

void AAbilityEffect::DestroyEffect()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_LifeTimeTimer);
	MarkPendingKill();
}
