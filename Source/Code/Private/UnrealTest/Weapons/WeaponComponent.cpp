// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values for this component's properties
//This weapon will be an implementation of an interface
//and this will be the "ray weapon"

/*Parent weapon as a component as this is intendet to be attachet to the character
and in the future may be new weapons with new caracteristics*/
UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	//Initialize projectile class
	ProjectileClass = AUnrealTestProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

void UWeaponComponent::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &UWeaponComponent::StopFire, FireRate, false);
		HandleFire();
	}
}

void UWeaponComponent::StopFire()
{
	bIsFiringWeapon = false;
}

void UWeaponComponent::HandleFire_Implementation()
{
	FVector spawnLocation = GetOwner()->GetActorLocation() + (Cast<AUnrealTestCharacter>(GetOwner())->GetControlRotation().Vector() * 100.0f) + (GetOwner()->GetActorUpVector() * 50.0f);
	FRotator spawnRotation = Cast<AUnrealTestCharacter>(GetOwner())->GetControlRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = Cast<AUnrealTestCharacter>(GetOwner())->GetInstigator();
	spawnParameters.Owner = GetOwner();

	AUnrealTestProjectile* spawnedProjectile = GetWorld()->SpawnActor<AUnrealTestProjectile>(spawnLocation, spawnRotation, spawnParameters);
	
}
