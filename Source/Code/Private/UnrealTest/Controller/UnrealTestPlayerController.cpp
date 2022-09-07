// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Controller/UnrealTestPlayerController.h"
#include "UnrealTest/Character/Hero/UnrealTestHeroBase.h"

void AUnrealTestPlayerController::SetTeamIndex(int NewIndex)
{
	TeamIndex = NewIndex;

	if(!PossessedHero)
	{
		if (!GetPawn())
		{
			UE_LOG(LogTemp, Display, TEXT("%s: function called, but no pawn is possesed yet, controller's team index updated for when possession happens"), __FUNCTION__);
			return;
		}

		PossessedHero = GetPawn<AUnrealTestHeroBase>();
		if (!ensureMsgf(PossessedHero, TEXT("%s: Could not cast NewIndex to AUnrealTestHeroBase. Controlled Pawn should be of class AUnrealTestHeroBase."), __FUNCTION__))
		{ return; }

		//PossessedHero->TeamIndex = TeamIndex;
		PossessedHero->SetTeamColor(NewIndex);
	}
}

void AUnrealTestPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (!ensureMsgf(TeamIndex != 0, TEXT("%s: Tried to set team index but it was 0. Team index should be greater or lower than 0"), __FUNCTION__))
	{ return; }

	if (!PossessedHero)
	{
		PossessedHero = GetPawn<AUnrealTestHeroBase>();
		if (!ensureMsgf(PossessedHero, TEXT("%s: Could not cast NewIndex to AUnrealTestHeroBase. Controlled Pawn should be of class AUnrealTestHeroBase."), __FUNCTION__))
		{ return; }
	}

	//PossessedHero->TeamIndex = TeamIndex;
	PossessedHero->SetTeamColor(TeamIndex);
}
