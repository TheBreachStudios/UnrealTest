// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealTestProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class UNREALTEST_API AUnrealTestProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnrealTestProjectile();

	/** Get projectile mesh component */
	UStaticMeshComponent* GetMeshComp() const { return MeshComp; }

	/** Get projectile collision component */
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Get projectile movement component */
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	//** Called when collision component overlaps something */
	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	/** Projectile mesh. Used only for visuals */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* MeshComp;

	/** Projectile collision component. Used for collision detection */ 
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
};
