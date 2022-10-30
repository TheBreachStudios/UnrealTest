// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALTEST_API Team
{
public:
	Team(int32 lives);
	~Team() = default;

	FORCEINLINE int GetTeamLives() const { return TeamLives; }
	FORCEINLINE int GetNumTeammates() const { return TeammatesPtrArray.Num(); }
	FORCEINLINE TArray<APlayerController*> GetTeammates() const { return TeammatesPtrArray; }

	void AssignTeammate(APlayerController* teammate);
	void RemoveTeammate(APlayerController* teammate);

	bool IsTeammateReady(APlayerController* teammate) const;
	bool AllTeammatesReady() const;
	void SetTeammateReadiness(APlayerController* teammate, bool isReady);

protected:
	
	int32 TeamLives = 0;
	TArray<APlayerController*> TeammatesPtrArray;	
	TMap<APlayerController*, bool> TeammatesReadyMap;
};
