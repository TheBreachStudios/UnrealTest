#include "UnrealTest/Weapon/Projectile/UnrealTestProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "UnrealTest/Weapon/UnrealTestProjectileGun.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"

AUnrealTestProjectile::AUnrealTestProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AUnrealTestProjectile::OnProjectileBeginOverlap);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AUnrealTestProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{ return; }

	if (ensureMsgf(OtherActor, TEXT("Projectile overlap began, but OtherActor was null")))
	{ return; }

	AUnrealTestProjectileGun* OwnerGun = GetOwner<AUnrealTestProjectileGun>();
	if (ensureMsgf(OwnerGun, TEXT("Projectile overlap began, but owner gun was null")))
	{ return; }

	AUnrealTestCharacter* const Victim = Cast<AUnrealTestCharacter>(OtherActor);
	ensureMsgf(Victim, TEXT("Projectile overlap began, but other actor was not a AUnrealTestCharacter. Projectiles should not overlap anything else"));

	AController* InstigatorController = OwnerGun->GetInstigator()->GetInstigatorController();
	ensureMsgf(Victim, TEXT("Projectile overlap began, but could not get instigator controller"));

	FDamageEvent DamageEvent;
	Victim->TakeDamage(OwnerGun->GetDamage(), DamageEvent, InstigatorController, OwnerGun);
}
