#include "UnrealTest/Weapon/UnrealTestWeaponBase.h"

AUnrealTestWeaponBase::AUnrealTestWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;

	RootComponent = WeaponMesh;
	
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
}
