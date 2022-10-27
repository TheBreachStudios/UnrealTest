// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/HealthComponent.h"
#include "..\..\..\Public\UnrealTest\Character\HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetHealth();	
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
}


//void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

