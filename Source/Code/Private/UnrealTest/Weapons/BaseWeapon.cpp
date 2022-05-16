// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/BaseWeapon.h"

// Unreal Engine
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Game Project
#include "UnrealTest/Game/UnrealTestGameMode.h"

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

void ABaseWeapon::Shoot(UCameraComponent* CameraRef)
{
	if (!GetOwner()->HasAuthority()) { return; }

	if (!CameraRef) { return; }

	// Gets posible aiming point from crosshair position
	// Use double de weapon range to be sure that it will always impact on something that the weapon an reach

	FHitResult cameraHit;
	FVector cameraStart = CameraRef->GetComponentLocation();
	FVector cameraEnd = cameraStart + (CameraRef->GetForwardVector() * Range * 2.0f);
	bool cameraHitSuccess = GetWorld()->LineTraceSingleByChannel(cameraHit, cameraStart, cameraEnd, ECollisionChannel::ECC_Visibility);

	// If camera probing has not hit anything weapon should point to the furthest posible point

	FHitResult shootHit;
	FVector maxShootPoint = cameraHitSuccess ? cameraHit.Location : cameraEnd;
	FVector shootStart = WeaponShootPoint->GetComponentLocation();
	FVector shootDirection = maxShootPoint - shootStart;
	shootDirection.Normalize(0.0001);
	FVector shootEnd = shootStart + (shootDirection * Range);

	bool shootHitSuccess = GetWorld()->LineTraceSingleByChannel(shootHit, shootStart, shootEnd, ECollisionChannel::ECC_Visibility);

	if (shootHitSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABaseWeapon][LocalRole: %s][RemoteRole: %s] %s->%s shot %s!"), *UEnum::GetValueAsString(GetLocalRole()), *UEnum::GetValueAsString(GetRemoteRole()), *GetOwner()->GetName(), *GetName(), *shootHit.GetActor()->GetName());

		AUnrealTestGameMode* gameModeRef = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		
		gameModeRef->ProcessDamage(shootHit.GetActor(), Damage, GetOwner()->GetInstigatorController(), this, nullptr);

	}
}


bool ABaseWeapon::Multicast_ShootDebug_Validate(FVector ShootStart, FVector ShootEnd, FColor LineColor, FColor SphereColor)
{
	return true;
}

void ABaseWeapon::Multicast_ShootDebug_Implementation(FVector ShootStart, FVector ShootEnd, FColor LineColor, FColor SphereColor)
{
	DrawDebugLine(GetWorld(), ShootStart, ShootEnd, SphereColor, false, 15.f, 0, 3.f);
	DrawDebugSphere(GetWorld(), ShootEnd, 10.f, 12, LineColor, false, 15.f, 0, 3.f);
	DrawDebugSphere(GetWorld(), ShootStart, 10.f, 12, LineColor, false, 15.f, 0, 3.f);
}

#pragma endregion Functions