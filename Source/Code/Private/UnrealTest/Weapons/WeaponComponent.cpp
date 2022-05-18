// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"



// Sets default values for this component's properties
//This weapon will be an implementation of an interface
//and this will be the "ray weapon"

/*Parent weapon as a component as this is intendet to be attachet to the character
and in the future may be new weapons with new caracteristics*/
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Range = 0.0f;
	Distance = 0.0f;
	MaxAmmo = 0.0f;
	CurrentAmmo = 0.0f;

	ShootPoint = FVector::ZeroVector;
	ShootDirection = FVector::ZeroVector;
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bShooting) {
		ShootProjectile();
	}
	// ...
}

void UWeaponComponent::ShootProjectile() {
	//If is basicWeapon then is a simply rycast
	//Will be more weapons that will override this method
	FHitResult OutHit;

	FVector Start = Cast<AUnrealTestCharacter>(GetOwner())->GetFollowCamera()->GetComponentLocation();
	FVector ForwardVector = Cast<AUnrealTestCharacter>(GetOwner())->GetFollowCamera()->GetForwardVector();

	Start = Start + (ForwardVector * Cast<AUnrealTestCharacter>(GetOwner())->GetCameraBoom()->TargetArmLength);
}


void UWeaponComponent::Shoot() {
	//TO EXTERNAL ACCESS NEEDED GETTERS AND SETTERS
	bShooting = true;

}

void UWeaponComponent::StopShoot() {
	//TO EXTERNAL ACCESS NEEDED GETTERS AND SETTERS
	bShooting = false;
}