#include "UnrealTest/Character/Hero/UnrealTestHeroBase.h"

AUnrealTestHeroBase::AUnrealTestHeroBase()
{
}

float AUnrealTestHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}
