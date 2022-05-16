// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/BaseWeapon.h"

// Unreal Engine
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#pragma region Initialization
// Sets default values
ABaseWeapon::ABaseWeapon()
{
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	RootComponent = WeaponCollider;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComponent->SetupAttachment(RootComponent);

	WeaponShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPoint"));
	WeaponShootPoint->SetupAttachment(WeaponMeshComponent);
	WeaponShootPoint->SetIsReplicated(true);

	// Replication
    bReplicates = true;
    SetReplicatingMovement(true);
}
#pragma endregion Initialization

#pragma region Overrides
// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::PostInitProperties()
{
	Super::PostInitProperties();

	InitiliazeWeapon();
}

#if WITH_EDITOR
void ABaseWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitiliazeWeapon();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Overrides

#pragma region Functions
//Initialize weapons values
void ABaseWeapon::InitiliazeWeapon()
{
	WeaponMeshComponent->SetStaticMesh(WeaponMesh);
	WeaponShootPoint->SetWorldLocation(ShootPointOffset);
}
#pragma endregion Functions