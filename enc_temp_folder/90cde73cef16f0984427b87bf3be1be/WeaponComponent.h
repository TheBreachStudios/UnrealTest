// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		FVector ShootPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		FVector ShootDirection;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	float Range;
	float Distance;
	float MaxAmmo;
	float CurrentAmmo;


	bool bShooting;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ShootProjectile();
	void Shoot();
	void StopShoot();
		
};
