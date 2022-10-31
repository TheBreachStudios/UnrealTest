// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	bReplicates = true;

	Damage = 10.f;
	Range = 10000.f;
	AttackRate = 3;


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
