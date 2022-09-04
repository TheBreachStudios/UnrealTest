#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "UnrealTest/Weapon/Projectile/UnrealTestProjectile.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectileTest, "UnrealTest.Projectile", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectileTest::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	{
		/**
		* Checks if a newly-spawned projectile has a valid components
		*/
		AUnrealTestProjectile* Projectile = World->SpawnActor<AUnrealTestProjectile>();

		if(!Projectile->GetProjectileMovement())
		{
			AddError(TEXT("Newly-spawned projectile did not initialize its projectile movement component"));
		}

		if (!Projectile->GetCollisionComp())
		{
			AddError(TEXT("Newly-spawned projectile did not initialize its collision component"));
		}

		Projectile->Destroy();
	}
	return true;
}
