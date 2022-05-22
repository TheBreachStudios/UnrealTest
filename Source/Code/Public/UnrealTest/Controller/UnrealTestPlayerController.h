// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Game Project
#include "UnrealTestPlayerController.generated.h"

// Definition
class AUnrealTestCharacter;

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region Variables
protected:
	// Actual Pawn class we want to use.
	UPROPERTY(Replicated)
	TSubclassOf<APawn> PawnToUse;

	// Actual Pawn name we want to use.
	UPROPERTY(Replicated)
	FName PawnToUseName;
#pragma endregion Variables

#pragma region Initialization
// Initialization
public:
	// Constructor.
	AUnrealTestPlayerController(const FObjectInitializer& ObjectInitializer);

	// Setup replicated properties.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion Initialization

#pragma region Getters / Setters
// Getters / Setters
public:
	// Get the player pawn class.
	FORCEINLINE UClass* GetPlayerPawnClass() { return PawnToUse; }

	// Get the player pawn name.
	FORCEINLINE FName GetPlayerPawnClassName() { return PawnToUseName; }
#pragma endregion Getters / Setters

#pragma region Overrides
protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region Functions
// Functions
public:
	// Set Pawn Class On Server For This Controller.
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
	virtual void Server_SetPawn(TSubclassOf<APawn> InPawnClass, FName InPawnName);
	virtual void Server_SetPawn_Implementation(TSubclassOf<APawn> InPawnClass, FName InPawnName);
	virtual bool Server_SetPawn_Validate(TSubclassOf<APawn> InPawnClass, FName InPawnName);
#pragma endregion Functions
};
