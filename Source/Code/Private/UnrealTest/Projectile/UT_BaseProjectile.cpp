// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Projectile/UT_BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"

// Sets default values
AUT_BaseProjectile::AUT_BaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision Sphere
	SetSphere();

	//Movement Component
	SetProjectileMovement();

	Damage = 10.0f;
	Range = 1000.0f; //100m 
	bReplicates = true;
}

// Called when the game starts or when spawned
void AUT_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	//In case projectile doesn't collides with anything we need to destroy it
	float life = CalculateLifeSpan();
	SetLifeSpan(life); 
}

// Called every frame
void AUT_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUT_BaseProjectile::OnProjectileImpact(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		//Apply Damage to Char and check is other team
		if (AUnrealTestCharacter* otherAsUTChar = Cast<AUnrealTestCharacter>(OtherActor))
		{			
			if (AUnrealTestCharacter* ownerAsUTChar = Cast<AUnrealTestCharacter>(GetOwner()))
			{
				if (otherAsUTChar->GetPlayerTeam() != ownerAsUTChar->GetPlayerTeam())
				{
					otherAsUTChar->ReceiveDamage(Damage);
				}
			}
		}

		ProjectileMovementComponent->StopMovementImmediately();
		Destroy();
	}

}

void AUT_BaseProjectile::SetSphere()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(32.f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComponent->SetHiddenInGame(false);

	RootComponent = SphereComponent;

	//Registering the Projectile Impact function on a Hit event.
	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AUT_BaseProjectile::OnProjectileImpact);
	}
}

void AUT_BaseProjectile::SetProjectileMovement()
{
	//Definition for the Projectile Movement Component.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

float AUT_BaseProjectile::CalculateLifeSpan() const
{
	if (ProjectileMovementComponent)
	{
		return (Range / ProjectileMovementComponent->MaxSpeed);
	}

	return 0.0f;
}
