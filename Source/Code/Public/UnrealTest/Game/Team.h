// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALTEST_API Team
{
public:
	Team();
	~Team() = default;

	FORCEINLINE int32 GetNumTeammates() const { return TeammatesPtrArray.Num(); }
	FORCEINLINE TArray<APlayerController*> GetTeammates() const { return TeammatesPtrArray; }
	FORCEINLINE int32 GetTeamID() const { return TeamID; }
	bool IsPlayerInTeam(APlayerController* player) const;

	void AssignTeammate(APlayerController* teammate);
	void RemoveTeammate(APlayerController* teammate);

	bool IsTeammateReady(APlayerController* teammate) const;
	bool AllTeammatesReady() const;
	void SetTeammateReadiness(APlayerController* teammate, bool isReady);

protected:
	
	TArray<APlayerController*> TeammatesPtrArray;	
	TMap<APlayerController*, bool> TeammatesReadyMap;
	int32 TeamID;
};
