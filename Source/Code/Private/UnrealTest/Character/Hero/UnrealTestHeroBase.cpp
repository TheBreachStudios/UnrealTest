#include "UnrealTest/Character/Hero/UnrealTestHeroBase.h"

AUnrealTestHeroBase::AUnrealTestHeroBase()
{
	CurrentHealth = MaxHealth;
}

float AUnrealTestHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth < 0) CurrentHealth = 0;

	return DamageAmount;
}
