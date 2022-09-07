// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "UnrealTest/Controller/UnrealTestPlayerController.h"

AUnrealTestGameMode::AUnrealTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Hero/BP_ProjectileGunHero"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AUnrealTestPlayerController::StaticClass();
}

void AUnrealTestGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AUnrealTestPlayerController* NewUnrealTestPlayerController = Cast<AUnrealTestPlayerController>(NewPlayer);
	if(!ensureMsgf(NewUnrealTestPlayerController, TEXT("%s: Could not cast NewPlayer to AUnrealTestPlayerController. AUnrealTestPlayerController should be the default controller"), __FUNCTION__))
	{ return; }

	bool NoPlayers = (BlueTeamMembers.Num() == 0 && RedTeamMembers.Num() == 0);
	bool RedTeamSizeEqualOrGreaterThanBlueTeamSize = RedTeamMembers.Num() >= BlueTeamMembers.Num();

	if(NoPlayers || RedTeamSizeEqualOrGreaterThanBlueTeamSize)
	{
		BlueTeamMembers.Add(NewUnrealTestPlayerController);
		NewUnrealTestPlayerController->SetTeamIndex(1);
	}
	else
	{
		RedTeamMembers.Add(NewUnrealTestPlayerController);
		NewUnrealTestPlayerController->SetTeamIndex(-1);
	}
}
