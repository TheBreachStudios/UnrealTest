#include "UnrealTest/Character/Hero/UnrealTestProjectileGunHero.h"
#include "UnrealTest/Weapon/UnrealTestProjectileGun.h"

AUnrealTestProjectileGunHero::AUnrealTestProjectileGunHero()
{

}

void AUnrealTestProjectileGunHero::SpawnWeapon()
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = "ProjectileGun";
	SpawnParameters.Owner = SpawnParameters.Instigator = this;

	Weapon = World->SpawnActor<AUnrealTestProjectileGun>(DefaultWeaponClass, SpawnParameters);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HeadSocket"));
}
