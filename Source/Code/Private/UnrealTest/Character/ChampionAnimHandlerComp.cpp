// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UnrealTest/Character/HealthComponent.h"

UChampionAnimHandlerComp::UChampionAnimHandlerComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OwningActor = nullptr;
	MovementSpeed = 0.f;
	SidewaysMovementDirection = 0.f;
	IsInAir = false;
	IsDead = false;
}


void UChampionAnimHandlerComp::BeginPlay()
{
	Super::BeginPlay();

	OwningActor = GetOwner();
	check(OwningActor);

	BindHealthEvents();	
}


void UChampionAnimHandlerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMovementSpeed();
	UpdateSidewaysMovementDirection();
	UpdateIsInAir();
}

void UChampionAnimHandlerComp::UpdateMovementSpeed() 
{
	if (OwningActor == nullptr) { return; }

	FVector velocityVector = OwningActor->GetVelocity();
	MovementSpeed = velocityVector.Size();
}

void UChampionAnimHandlerComp::UpdateSidewaysMovementDirection() 
{
	if (OwningActor == nullptr) { return; }

	FVector velocityVector = OwningActor->GetVelocity();
	FRotator actorRotation = OwningActor->GetActorRotation();
	FVector rightVector = UKismetMathLibrary::GetRightVector(actorRotation);
	float vectorProjection = FVector::DotProduct(velocityVector, rightVector);
	SidewaysMovementDirection = vectorProjection;
}

void UChampionAnimHandlerComp::UpdateIsInAir() 
{
	if (OwningActor == nullptr) { return; }

	APawn* owningPawn = Cast<APawn>(OwningActor);
	if (owningPawn == nullptr) { return; }

	UPawnMovementComponent* pawnMovementComponent = owningPawn->GetMovementComponent();
	if (pawnMovementComponent == nullptr) { return; }

	IsInAir = pawnMovementComponent->IsFalling();
}

void UChampionAnimHandlerComp::BindHealthEvents()
{
	if (OwningActor == nullptr) { return; }

	UHealthComponent* healthComponent = Cast<UHealthComponent>(OwningActor->GetComponentByClass(UHealthComponent::StaticClass()));
	if (healthComponent == nullptr) { return; }

	healthComponent->OnHealthEmpty.AddUObject(this, &UChampionAnimHandlerComp::SetIsDead);
}

void UChampionAnimHandlerComp::SetIsDead()
{
	IsDead = true;
}

