// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/ShootingWeapon.h"
#include "Engine/World.h"

// Sets default values
AShootingWeapon::AShootingWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	ResetClipAmmo();
}

// Called when the game starts or when spawned
void AShootingWeapon::BeginPlay()
{
	AActor::BeginPlay();
	
	
}

void AShootingWeapon::UseWeapon()
{
}

void AShootingWeapon::Reload()
{
}

void AShootingWeapon::ResetClipAmmo()
{
	// TODO: Turn this into accessing the ammo management component
	CurrentClipAmmo = MaxClipAmmo;
}

void AShootingWeapon::Shoot()
{
}

bool AShootingWeapon::CheckHitScanTrace()
{
	return false;
}

//void AShootingWeapon::Tick(float DeltaTime)
//{
//	AActor::Tick(DeltaTime);
//
//}


