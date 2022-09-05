// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapon/UnrealTestWeaponBase.h"

AUnrealTestWeaponBase::AUnrealTestWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	FPWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPWeapon"));

	FPWeaponMesh->SetOnlyOwnerSee(false); // otherwise won't be visible in the multiplayer
	FPWeaponMesh->bCastDynamicShadow = false;
	FPWeaponMesh->CastShadow = false;
	FPWeaponMesh->SetupAttachment(RootComponent);
}
