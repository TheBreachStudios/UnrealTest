// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Enegine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Game Project
#include "BaseWeapon.generated.h"

// Definitions
class UBoxComponent;
class UCameraComponent;

UCLASS()
class UNREALTEST_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

#pragma region Configuration
// Configuration
public:
	// Weapon mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	UStaticMesh* WeaponMesh = nullptr;

	// Weapon range
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	float Range = 0.f;

	// Weapon damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	float Damage = 0.f;

	// Weapon firerate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	float Firerate = 0.f;

	// Weapon firerate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	FVector ShootPointOffset = FVector(0.f);
#pragma endregion Configuration

#pragma region Variables
// Variables
public:
	// Weapon collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* WeaponCollider = nullptr;

	// Weapon mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* WeaponMeshComponent = nullptr;

	// Weapon shootpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* WeaponShootPoint = nullptr;
#pragma endregion Variables

#pragma region Initialization
// Initialization
public:
	// Sets default values for this actor's properties
	ABaseWeapon();
#pragma endregion Initialization

#pragma region Overrides
// Overrides
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion Overrides

#pragma region Functions
// Functions
protected:
	//Initialize weapons values
	void InitiliazeWeapon();

	// Server shoot handle
	/*UFUNCTION(Server, Unreliable, WithValidation)
	void Server_Shoot(UCameraComponent* CameraRef);
	void Server_Shoot_Implementation(UCameraComponent* CameraRef);
	bool Server_Shoot_Validate(UCameraComponent* CameraRef);*/

	// Multicast shoot debug
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_ShootDebug(FVector ShootStart, FVector ShootEnd, FColor LineColor, FColor SphereColor);
	void Multicast_ShootDebug_Implementation(FVector ShootStart, FVector ShootEnd, FColor LineColor, FColor SphereColor);
	bool Multicast_ShootDebug_Validate(FVector ShootStart, FVector ShootEnd, FColor LineColor, FColor SphereColor);

public:
	// Server shoot handle
	void Shoot(UCameraComponent* CameraRef);
#pragma endregion Functions

};
