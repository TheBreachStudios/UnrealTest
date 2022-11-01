// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

UChampionAnimHandlerComp::UChampionAnimHandlerComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	MovementSpeed = 0.f;
	SidewaysMovementDirection = 0.f;
	IsInAir = false;
	IsDead = false;
	IsReloading = false;
}


void UChampionAnimHandlerComp::BeginPlay()
{
	Super::BeginPlay();

	BindToHealthEvents();
}

void UChampionAnimHandlerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Multicast_UpdateMovementProperties();
}

void UChampionAnimHandlerComp::Multicast_UpdateMovementProperties_Implementation()
{
	if (GetOwner() == nullptr) { return; }

	// MovementSpeed
	FVector velocityVector = GetOwner()->GetVelocity();
	MovementSpeed = velocityVector.Size();

	// SidewaysMovement
	FRotator actorRotation = GetOwner()->GetActorRotation();
	FVector rightVector = UKismetMathLibrary::GetRightVector(actorRotation);
	float vectorProjection = FVector::DotProduct(velocityVector, rightVector);
	SidewaysMovementDirection = vectorProjection;

	// IsInAir
	APawn* owningPawn = Cast<APawn>(GetOwner());
	if (owningPawn != nullptr) 
	{
		UPawnMovementComponent* pawnMovementComponent = owningPawn->GetMovementComponent();
		if (pawnMovementComponent != nullptr) 
		{ 
			IsInAir = pawnMovementComponent->IsFalling();
		}		
	}

	// UpperBodyBlending
	UpperBodyBlending = (!IsDead && (IsReloading || IsShooting)) ? 1.f : 0.f;
}

void UChampionAnimHandlerComp::Multicast_SetIsDead_Implementation()
{
	IsDead = true;
}

void UChampionAnimHandlerComp::BindToHealthEvents()
{
	if (GetOwner() == nullptr) { return; }

	AChampionCharacter* champion = Cast<AChampionCharacter>(GetOwner());
	if (champion != nullptr)
	{ 
		champion->OnChampionDeathEvent.AddUObject(this, &UChampionAnimHandlerComp::Multicast_SetIsDead);
	}
}

void UChampionAnimHandlerComp::Multicast_ResetAnimation_Implementation()
{
	MovementSpeed = 0.f;
	SidewaysMovementDirection = 0.f;
	UpperBodyBlending = 0.f;
	IsDead = false;
	IsReloading = false;
	IsShooting = false;
}

void UChampionAnimHandlerComp::BindToWeaponEvents(ABaseWeapon* weapon)
{
	ABaseShootingWeapon* shootingWeapon = Cast<ABaseShootingWeapon>(weapon);
	if (shootingWeapon != nullptr)
	{
		shootingWeapon->OnStartedReloadingEvent.AddUObject(this, &UChampionAnimHandlerComp::Multicast_SetStartedReloading);
		shootingWeapon->OnEndedReloadingEvent.AddUObject(this, &UChampionAnimHandlerComp::Multicast_SetEndedReloading);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to bind to the Weapon Events!"));
	}
}

void UChampionAnimHandlerComp::Multicast_SetStartedReloading_Implementation()
{
	IsReloading = true;
}

void UChampionAnimHandlerComp::Multicast_SetEndedReloading_Implementation()
{
	IsReloading = false;
}

void UChampionAnimHandlerComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UChampionAnimHandlerComp, MovementSpeed);
	DOREPLIFETIME(UChampionAnimHandlerComp, SidewaysMovementDirection);
	DOREPLIFETIME(UChampionAnimHandlerComp, UpperBodyBlending);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsInAir);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsDead);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsReloading);
	DOREPLIFETIME(UChampionAnimHandlerComp, IsShooting);
}


