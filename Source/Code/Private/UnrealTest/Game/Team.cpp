// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/Team.h"

Team::Team(int32 lives)
{
	TeamLives = lives;
}

void Team::AssignTeammate(APlayerController* teammate)
{
	if (!TeammatesReadyMap.Contains(teammate))
	{
		TeammatesPtrArray.Add(teammate);
		TeammatesReadyMap.Add(teammate, false);
	}
}

void Team::RemoveTeammate(APlayerController* teammate)
{
	if (TeammatesReadyMap.Contains(teammate))
	{
		TeammatesPtrArray.Remove(teammate);
		TeammatesReadyMap.Remove(teammate);
	}
}

bool Team::IsTeammateReady(APlayerController* teammate) const
{
	if (TeammatesReadyMap.Contains(teammate))
	{
		return TeammatesReadyMap[teammate];
	}
	return false;
}

bool Team::AllTeammatesReady() const
{
	for (int i = 0; i < TeammatesPtrArray.Num(); i++)
	{
		if (!TeammatesReadyMap[TeammatesPtrArray[i]])
		{
			return false;
		}
	}
	return true;
}

void Team::SetTeammateReadiness(APlayerController* teammate, bool isReady)
{
	if (TeammatesReadyMap.Contains(teammate))
	{
		TeammatesReadyMap[teammate] = isReady;
	}
}
