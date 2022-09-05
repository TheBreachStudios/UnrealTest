#include "UnrealTest/Character/Hero/UnrealTestProjectileGunHero.h"
#include "UnrealTest/Weapon/UnrealTestProjectileGun.h"

AUnrealTestProjectileGunHero::AUnrealTestProjectileGunHero()
{
	SetupWeapon();
}

void AUnrealTestProjectileGunHero::SetupWeapon()
{
	Weapon = CreateDefaultSubobject<AUnrealTestProjectileGun>(TEXT("ProjectileGun"));
	Weapon->SetInstigator(this);

	// Attach to socket...
}
