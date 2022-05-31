// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UT_BaseProjectile.generated.h"

class USphereComponent; 
class UStaticMeshComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileImpact, float, Damage);

UCLASS()
class UNREALTEST_API AUT_BaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUT_BaseProjectile();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	// Movement component for handling projectile movement.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	//The damage dealt by this projectile.
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Range;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnProjectileImpact OnProjectileImpactEvent;

	UFUNCTION(Category = "Projectile")
	void OnProjectileImpact(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	//Set Sphere dimensions 
	void SetSphere();

	//Set Projectile Default Movement
	void SetProjectileMovement();

	//Calculates projectile life (Range/Speed)
	float CalculateLifeSpan() const;
};
