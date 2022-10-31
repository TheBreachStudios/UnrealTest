// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Damage = 10.f;
	Range = 10000.f;
	AttackRate = 3;

	bReplicates = true;
}

void ABaseWeapon::TryUseWeapon()
{
}

void ABaseWeapon::StopUsingWeapon()
{
}

void ABaseWeapon::ResetWeapon()
{
}

void ABaseWeapon::SetWeaponOwner(APawn* pawn)
{
	OwningPawn = pawn;
}

bool ABaseWeapon::CanUseWeapon()
{
	return true;
}
