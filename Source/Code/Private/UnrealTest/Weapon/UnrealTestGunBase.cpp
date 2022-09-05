// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapon/UnrealTestGunBase.h"

AUnrealTestGunBase::AUnrealTestGunBase()
{
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(FPWeaponMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

void AUnrealTestGunBase::OnAttack()
{
	Fire();
}
