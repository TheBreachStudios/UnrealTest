// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"



// Sets default values for this component's properties

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
		/*
		* 		const FVector EndTrace = ShootPoint + ShootDirection * Distance; // and trace end is the camera location
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;


		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function


		IUsable* usable = InterfaceCast<IUsable>(Hit.GetActor()); // we cast the hit actor to the IUsable interface
		if (usable) // we are looking to a usable object
		{
			UseFocus = usable; // as the actor under crosshairs is a usable actor, we store it for the hud.
		}
		else
		{
			UseFocus = NULL; // nothing, so we set the UseFocus pointer to NULL, so it wont give problems
		}

		*/
		

	}
	// ...
}

void UWeaponComponent::Raycast() {
	FHitResult OutHit;

	FVector Start = Cast<AUnrealTestCharacter>(GetOwner())->GetFollowCamera()->GetComponentLocation();
	FVector ForwardVector = Cast<AUnrealTestCharacter>(GetOwner())->GetFollowCamera()->GetForwardVector();

	Start = Start + (ForwardVector * Cast<AUnrealTestCharacter>(GetOwner())->GetCameraBoom()->TargetArmLength);
}


void UWeaponComponent::Shoot() {
	bShooting = true;

}

void UWeaponComponent::StopShoot() {
	bShooting = false;
}