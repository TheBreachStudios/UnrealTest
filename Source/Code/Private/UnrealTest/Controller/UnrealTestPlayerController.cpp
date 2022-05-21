// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Controller/UnrealTestPlayerController.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

// Game Project
#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"


#pragma region Initialization
// Initialization

// Constructor
AUnrealTestPlayerController::AUnrealTestPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Ensures replication
	bReplicates = true;
}
#pragma endregion Initialization

#pragma region Overrides
// Overrides

// Begin play
void AUnrealTestPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
#pragma endregion Overrides

#pragma region Functions
//Functions

// Set Pawn Class On Server For This Controller validation
bool AUnrealTestPlayerController::Server_SetPawn_Validate(TSubclassOf<APawn> InPawnClass, FName InPawnName)
{
	return true;
}

// Set Pawn Class On Server For This Controller
void AUnrealTestPlayerController::Server_SetPawn_Implementation(TSubclassOf<APawn> InPawnClass, FName InPawnName)
{
	PawnToUse = InPawnClass;
	PawnToUseName = InPawnName;

	// Just in case we didn't get the PawnClass on the Server in time... 
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

// Replication
void AUnrealTestPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnrealTestPlayerController, PawnToUse);
	DOREPLIFETIME(AUnrealTestPlayerController, PawnToUseName);
}
#pragma endregion Functions