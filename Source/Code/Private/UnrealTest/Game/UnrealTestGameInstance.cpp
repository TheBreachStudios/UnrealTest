// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/UnrealTestGameInstance.h"

// Game Project
#include "UnrealTest/Character/UnrealTestCharacter.h"

#pragma region Functions
// Functions

// Server change default pawn definition
bool UUnrealTestGameInstance::Server_SetDefaultPawn_Validate(TSubclassOf<AUnrealTestCharacter> NewDefaultPawn, FName NewDefaultPawnName)
{
	return true;
}

// Server change default pawn definition
void UUnrealTestGameInstance::Server_SetDefaultPawn_Implementation(TSubclassOf<AUnrealTestCharacter> NewDefaultPawn, FName NewDefaultPawnName) {
	DefaultPawn = NewDefaultPawn;
	DefaultPawnName = NewDefaultPawnName;
}
#pragma endregion Functions