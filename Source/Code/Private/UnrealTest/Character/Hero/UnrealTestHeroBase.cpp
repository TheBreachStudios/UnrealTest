#include "UnrealTest/Character/Hero/UnrealTestHeroBase.h"
#include "UnrealTest/Weapon/UnrealTestWeaponBase.h"

AUnrealTestHeroBase::AUnrealTestHeroBase()
{
	CurrentHealth = MaxHealth;
}

void AUnrealTestHeroBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void AUnrealTestHeroBase::UseWeapon()
{
	Weapon->Use();
}

float AUnrealTestHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth < 0) CurrentHealth = 0;

	return DamageAmount;
}
