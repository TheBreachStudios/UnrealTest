// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Weapons/UnrealTestProjectile.h"
#include "WeaponComponent.generated.h"


class AUnrealTestCharacter;
class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()




public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		FVector ShootPoint;



protected:

	// Called when the game starts
	//virtual void BeginPlay() override;

	float Range;
	float Distance;
	float MaxAmmo;
	float CurrentAmmo;

	bool bShooting;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
		TSubclassOf<class AUnrealTestProjectile> ProjectileClass;

	/** Delay between shots in seconds. Used to control fire rate for our test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** If true, we are in the process of firing projectiles. */
	bool bIsFiringWeapon;


	/** Function for ending weapon fire. Once this is called, the player can use StartFire again.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
		void HandleFire();

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;


public:	

	/** Function for beginning weapon fire that the unrealtestChracter will call with left click*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire();
		
};
