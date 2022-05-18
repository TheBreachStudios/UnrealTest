// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponComponent.h"

// Sets default values for this component's properties

/*Parent weapon as a component as this is intendet to be attachet to the character
and in the future may be new weapons with new caracteristics*/
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	// ...
}


void UWeaponComponent::Shoot() {

}