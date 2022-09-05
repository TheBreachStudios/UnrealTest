#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Weapon/UnrealTestGunBase.h"
#include "UnrealTestProjectileGun.generated.h"

/**
 * Class representing a projectile gun. Projectile gun meaning it is not hitscan and spawns and shoots actual projectiles.
 */
UCLASS()
class UNREALTEST_API AUnrealTestProjectileGun : public AUnrealTestGunBase
{
	GENERATED_BODY()

public:
	virtual void Fire();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AUnrealTestProjectile> ProjectileClass;
};
