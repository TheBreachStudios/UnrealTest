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
	Server_UseWeapon();
}

void AUnrealTestHeroBase::Server_UseWeapon_Implementation()
{
	Weapon->Use();
}

bool AUnrealTestHeroBase::Server_UseWeapon_Validate()
{
	return true;
}

float AUnrealTestHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth < 0) CurrentHealth = 0;

	return DamageAmount;
}
