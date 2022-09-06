#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/Hero/UnrealTestHeroBase.h"
#include "UnrealTestProjectileGunHero.generated.h"

class AUnrealTestProjectileGun;

/**
 * Class representing a hero that uses a projectile gun. Projectile gun meaning it is not hitscan and spawns and shoots actual projectiles.
 */
UCLASS()
class UNREALTEST_API AUnrealTestProjectileGunHero : public AUnrealTestHeroBase
{
	GENERATED_BODY()

public:
	AUnrealTestProjectileGunHero();

protected:
	virtual void SpawnWeapon() override;
};
