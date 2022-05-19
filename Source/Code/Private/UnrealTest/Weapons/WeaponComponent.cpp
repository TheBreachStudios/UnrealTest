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
	Range = 0.f;
	Distance = 0.f;
	MaxAmmo = 0.f;
	CurrentAmmo = 0.f;

	ShootPoint = FVector::ZeroVector;

	bShooting = false;
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	Distance = 5000.f;
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

	//FVector Start = Cast<AUnrealTestCharacter>(GetOwner())->GetFollowCamera()->GetComponentLocation();
	FVector ForwardVector = Cast<AUnrealTestCharacter>(GetOwner())->GetFollowCamera()->GetForwardVector();

	//Start = Start + (ForwardVector * Cast<AUnrealTestCharacter>(GetOwner())->GetCameraBoom()->TargetArmLength);
	
	// Using the weapon distance
	FVector End = ShootPoint + (ForwardVector * Distance);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	// Draw Raycast for debug
	DrawDebugLine(GetWorld(), ShootPoint, End, FColor::Green, false, 1, 0, 1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, ShootPoint, End, ECC_Visibility, CollisionParams);

	/*if (isHit) {
		OutHit.GetActor()->Destroy();
	}*/
}


void UWeaponComponent::Shoot() {
	SetShoot(true);

}

void UWeaponComponent::StopShoot() {
	SetShoot(false);
}

void UWeaponComponent::SetShoot(bool shooting)
{
	bShooting = shooting;
}

bool UWeaponComponent::GetShoot() const
{
	return bShooting;
}
