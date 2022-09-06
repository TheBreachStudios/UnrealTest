#include "UnrealTest/Weapon/UnrealTestProjectileGun.h"
#include "UnrealTest/Weapon/Projectile/UnrealTestProjectile.h"

void AUnrealTestProjectileGun::Fire()
{
	if (!ensureMsgf(ProjectileClass, TEXT("Tried to fire projectile gun, but ProjectileClass is null")))
	{ return; }

	UWorld* const World = GetWorld();
	if(!ensureMsgf(World, TEXT("Tried to fire projectile gun, but World was null")))
	{ return; }

	const FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();
	const FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

	FActorSpawnParameters ProjectileSpawnParams;
	ProjectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ProjectileSpawnParams.Instigator = GetOwner<APawn>();
	ProjectileSpawnParams.Owner = this;

	World->SpawnActor<AUnrealTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ProjectileSpawnParams);
}
