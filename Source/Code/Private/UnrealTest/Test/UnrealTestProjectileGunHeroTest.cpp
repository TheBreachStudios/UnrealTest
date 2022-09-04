#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "UnrealTest/Character/Hero/UnrealTestProjectileGunHero.h"

#include "UnrealTest/Weapon/UnrealTestWeaponBase.h"
#include "UnrealTest/Weapon/UnrealTestProjectileGun.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectileGunHeroTest, "UnrealTest.ProjectileGunHero", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectileGunHeroTest::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	{
		/**
		* Checks if a newly-spawned hero correctly initializes its health values
		*/
		AUnrealTestProjectileGunHero* Hero = World->SpawnActor<AUnrealTestProjectileGunHero>();

		if (Hero->GetMaxHealth() <= 0)
		{
			/*
			* This ideally would compare the value to the one specified in a data table to make it more accesible to game designers.
			* We would make sure it is bigger than 0 there.
			*/
			AddError(TEXT("Newly-spawned projectile gun hero's maximum health must be greater than 0"));
		}
		else if (Hero->GetCurrentHealth() != Hero->GetMaxHealth())
		{
			AddError(TEXT("Newly-spawned projectile gun hero's health must start at max health"));
		}
		Hero->Destroy();
	}

	{
		/**
		* Checks if a hero correctly takes damage.
		* (Considering that the hero could be avoiding damage using some kind of ability)
		*/
		AUnrealTestProjectileGunHero* Hero = World->SpawnActor<AUnrealTestProjectileGunHero>();

		float previousHealth = Hero->GetMaxHealth();
		FDamageEvent damageEvent;
		float damageTaken = Hero->TakeDamage(1, damageEvent, nullptr, nullptr);

		if (Hero->GetCurrentHealth() != (previousHealth - damageTaken))
		{ 
			AddError(TEXT("Newly-spawned projectile gun hero's current health did not decrease after taking damage not equal to 0"));
		}
		else if(Hero->GetCurrentHealth() < 0)
		{
			AddError(TEXT("Newly-spawned projectile gun hero's current health is lower than 0 after applying damage"));
		}
		Hero->Destroy();
	}

	{
		/**
		* Checks if a projectile gun is spawned when spawning a projectile gun hero
		*/
		AUnrealTestProjectileGunHero* Hero = World->SpawnActor<AUnrealTestProjectileGunHero>();

		AUnrealTestWeaponBase* Weapon = Hero->GetWeapon();
		if (!Weapon)
		{
			AddError(TEXT("Newly-spawned projectile gun hero has no weapon"));
		}
		else if (!Weapon->IsA(AUnrealTestProjectileGun::StaticClass()))
		{
			AddError(TEXT("Newly-spawned projectile gun hero's weapon is not a projectile gun"));
		}

		Hero->Destroy();
	}

	return true;
}
