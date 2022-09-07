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

void AUnrealTestHeroBase::ModifyCurrentHealth(float Amount)
{
	CurrentHealth += Amount;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth; 
	}

	if (CurrentHealth <= 0)
	{
		Die();
	}
}

float AUnrealTestHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ModifyCurrentHealth(-DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AUnrealTestHeroBase::Die()
{
	UE_LOG(LogTemp, Display, TEXT("A Character has died"));
	Respawn();
}

void AUnrealTestHeroBase::Respawn()
{
	UE_LOG(LogTemp, Display, TEXT("A Character has respawned"));
}
