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

	FVector Location(0, 50, 0);
	Weapon->SetActorRelativeLocation(Location);

	FRotator Rotator(270, 0, 0);
	Weapon->SetActorRelativeRotation(Rotator);
}
