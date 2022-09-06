#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "UnrealTest/Weapon/UnrealTestProjectileGun.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectileGunTest, "Example.Hero", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectileGunTest::RunTest(const FString& Parameters)
{
	UWorld* const World = FAutomationEditorCommonUtils::CreateNewMap();

	return true;
}
