// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Net/UnrealNetwork.h"

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

	verify((OwningActor = GetOwner()) != nullptr);

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
	if (OwningActor == nullptr) { return; }

	FVector velocityVector = OwningActor->GetVelocity();
	MovementSpeed = velocityVector.Size();
}

void UChampionAnimHandlerComp::Multicast_UpdateSidewaysMovementDirection_Implementation()
{
	if (OwningActor == nullptr) { return; }

	FVector velocityVector = OwningActor->GetVelocity();
	FRotator actorRotation = OwningActor->GetActorRotation();
	FVector rightVector = UKismetMathLibrary::GetRightVector(actorRotation);
	float vectorProjection = FVector::DotProduct(velocityVector, rightVector);
	SidewaysMovementDirection = vectorProjection;
}

void UChampionAnimHandlerComp::Multicast_UpdateIsInAir_Implementation()
{
	if (OwningActor == nullptr) { return; }

	APawn* owningPawn = Cast<APawn>(OwningActor);
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
	check(OwningActor != nullptr);
	if (OwningActor == nullptr) { return; }

	UHealthComponent* healthComponent = Cast<UHealthComponent>(OwningActor->GetComponentByClass(UHealthComponent::StaticClass()));
	if (healthComponent == nullptr) { return; }

	healthComponent->OnHealthEmpty.AddUObject(this, &UChampionAnimHandlerComp::Multicast_SetIsDead);
}

void UChampionAnimHandlerComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UChampionAnimHandlerComp, MovementSpeed);
	DOREPLIFETIME(UChampionAnimHandlerComp, SidewaysMovementDirection);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsInAir);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsDead);
}

//void UChampionAnimHandlerComp::Client_PrintPropertyValues_Implementation() const
//{
//	FString tempStr = FString::Printf(TEXT("MovementSpeed: %f | SidewaysMovementDirection: %f | IsInAir: %d | IsDead: %d"), MovementSpeed, SidewaysMovementDirection, IsInAir, IsDead);
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, tempStr);
//}

