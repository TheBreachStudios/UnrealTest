// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Game Project
#include "UnrealTestGameInstance.generated.h"

// Definitions
class AUnrealTestCharacter;

/**
 * 
 */
UCLASS()
class UNREALTEST_API UUnrealTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
#pragma region Variables
// Variables
public:
	// Default pawn class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Champion")
	TSubclassOf<AUnrealTestCharacter> DefaultPawn = nullptr;
	
	// Default pawn name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Champion")
	FName DefaultPawnName = NAME_None;
#pragma endregion Variables

#pragma region Functions
// Functions
public:

	// Server change default pawn definition
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_SetDefaultPawn(TSubclassOf<AUnrealTestCharacter> NewDefaultPawn, FName NewDefaultPawnName);
	void Server_SetDefaultPawn_Implementation(TSubclassOf<AUnrealTestCharacter> NewDefaultPawn, FName NewDefaultPawnName);
	bool Server_SetDefaultPawn_Validate(TSubclassOf<AUnrealTestCharacter> NewDefaultPawn, FName NewDefaultPawnName);
#pragma endregion Functions

};
