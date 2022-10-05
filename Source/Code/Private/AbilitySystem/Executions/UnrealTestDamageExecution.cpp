// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/UnrealTestDamageExecution.h"
#include "AbilitySystem/UnrealTestAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/UnrealTestAttributeSet.h"
#include "System/UnrealTestGameplayTags.h"

UUnrealTestDamageExecution::UUnrealTestDamageExecution()
{
}

void UUnrealTestDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FUnrealTestGameplayTags& GameplayTags = FUnrealTestGameplayTags::Get();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float Damage = Spec.GetSetByCallerMagnitude(GameplayTags.Gameplay_Damage, false, 0.f);
	Damage = FMath::Max<float>(Damage, 0.f);

	if (Damage > 0.f)
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UUnrealTestAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage));

#endif // #if WITH_SERVER_CODE
}
