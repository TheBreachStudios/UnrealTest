// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UT_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AUT_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AUT_PlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Team ID
	UPROPERTY(ReplicatedUsing = OnRep_TeamNumberChanged)
	int32 TeamNumber;
	
	UFUNCTION()
	void OnRep_TeamNumberChanged();
	
	//Times has died this pawn
	UPROPERTY(Replicated)
	int32 NumDeaths;

	// Player Died
	void ScoreDeath(const int32 Team);

	void AddDeath(const int32 Team);
	
	//Sets Team
	void SetTeamNum(int32 NewTeamNumber);
	
	//Get Team
	UFUNCTION(BlueprintCallable)
	int32 GetTeamNum() const;
	
	//Updates Mesh Colors With the team id -- Red or Blue
	void UpdateTeamColors();

	virtual void ClientInitialize(class AController* InController) override;
	
};
