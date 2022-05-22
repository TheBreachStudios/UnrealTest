// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Controller/UnrealTestPlayerController.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

// Game Project
#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"


#pragma region Initialization
// Constructor.
AUnrealTestPlayerController::AUnrealTestPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Ensures replication
	bReplicates = true;
}

// Setup replicated properties.
void AUnrealTestPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnrealTestPlayerController, PawnToUse);
	DOREPLIFETIME(AUnrealTestPlayerController, PawnToUseName);
}
#pragma endregion Initialization

#pragma region Overrides
// Called when the game starts or when spawned.
void AUnrealTestPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
#pragma endregion Overrides

#pragma region Functions
// Set Pawn Class On Server For This Controller validation.
bool AUnrealTestPlayerController::Server_SetPawn_Validate(TSubclassOf<APawn> InPawnClass, FName InPawnName)
{
	return true;
}

// Set Pawn Class On Server For This Controller.
void AUnrealTestPlayerController::Server_SetPawn_Implementation(TSubclassOf<APawn> InPawnClass, FName InPawnName)
{
	PawnToUse = InPawnClass;
	PawnToUseName = InPawnName;

	// Ensure actor has the propper info by forcing a restart. 
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}
#pragma endregion Functions