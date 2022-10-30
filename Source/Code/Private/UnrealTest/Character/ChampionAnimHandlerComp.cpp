// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Net/UnrealNetwork.h"

UChampionAnimHandlerComp::UChampionAnimHandlerComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	MovementSpeed = 0.f;
	SidewaysMovementDirection = 0.f;
	IsInAir = false;
	IsDead = false;
}


void UChampionAnimHandlerComp::BeginPlay()
{
	Super::BeginPlay();

	BindHealthEvents();	
}


void UChampionAnimHandlerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Multicast_UpdateMovementSpeed();
	Multicast_UpdateSidewaysMovementDirection();
	Multicast_UpdateIsInAir();
}

void UChampionAnimHandlerComp::Multicast_UpdateMovementSpeed_Implementation()
{
	if (GetOwner() == nullptr) { return; }

	FVector velocityVector = GetOwner()->GetVelocity();
	MovementSpeed = velocityVector.Size();
}

void UChampionAnimHandlerComp::Multicast_UpdateSidewaysMovementDirection_Implementation()
{
	if (GetOwner() == nullptr) { return; }

	FVector velocityVector = GetOwner()->GetVelocity();
	FRotator actorRotation = GetOwner()->GetActorRotation();
	FVector rightVector = UKismetMathLibrary::GetRightVector(actorRotation);
	float vectorProjection = FVector::DotProduct(velocityVector, rightVector);
	SidewaysMovementDirection = vectorProjection;
}

void UChampionAnimHandlerComp::Multicast_UpdateIsInAir_Implementation()
{
	if (GetOwner() == nullptr) { return; }

	APawn* owningPawn = Cast<APawn>(GetOwner());
	if (owningPawn == nullptr) { return; }

	UPawnMovementComponent* pawnMovementComponent = owningPawn->GetMovementComponent();
	if (pawnMovementComponent == nullptr) { return; }

	IsInAir = pawnMovementComponent->IsFalling();
}

void UChampionAnimHandlerComp::Multicast_SetIsDead_Implementation()
{
	IsDead = true;
}

void UChampionAnimHandlerComp::BindHealthEvents()
{
	if (GetOwner() == nullptr) { return; }

	UHealthComponent* healthComponent = Cast<UHealthComponent>(GetOwner()->GetComponentByClass(UHealthComponent::StaticClass()));
	if (healthComponent != nullptr) 
	{ 
		healthComponent->OnHealthEmptyEvent.AddUObject(this, &UChampionAnimHandlerComp::Multicast_SetIsDead);		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to bind to the HealthComponent Events!"));
	}
}

void UChampionAnimHandlerComp::Multicast_ResetAnimation_Implementation()
{
	IsDead = false;
}

void UChampionAnimHandlerComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UChampionAnimHandlerComp, MovementSpeed);
	DOREPLIFETIME(UChampionAnimHandlerComp, SidewaysMovementDirection);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsInAir);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsDead);
}

