// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "UnrealTest/Character/ChampionAudioComponent.h"
#include "UnrealTest/Weapons/BaseWeapon.h"
#include "UnrealTest/Weapons/BaseShootingWeapon.h"
#include "UnrealTest/Weapons/WeaponAudioComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"//

AChampionCharacter::AChampionCharacter()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	SetupHealthComponent();
	AnimHandler = CreateDefaultSubobject<UChampionAnimHandlerComp>(TEXT("ChampionAnimationHandlerComponent"));
	AudioComponent = CreateDefaultSubobject<UChampionAudioComponent>(TEXT("ChampionAudioComponent"));

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotingVFX(TEXT("/Game/StarterContent/Particles/P_Explosion"));
	//if (ShotingVFX.Succeeded())
	//{
	//	ShotVFX = ShotingVFX.Object;
	//}
}

void AChampionCharacter::Multicast_ResetChampionCharacter_Implementation()
{
	HealthComponent->ResetCurrentHealth();
	AnimHandler->Multicast_ResetAnimation();
	AudioComponent->Multicast_ResetAudio();
	if (HasWeapon)
	{
		Weapon->ResetWeapon();
	}
}

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();
	TryFindWeapon();
	UE_LOG(LogTemp, Warning, TEXT("Champion Begin Play!"));
}

void AChampionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ShootBinding(PlayerInputComponent);
	ReloadBinding(PlayerInputComponent);
}

void AChampionCharacter::ShootBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AChampionCharacter::ShootingStarted);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AChampionCharacter::ShootingStopped);
}

void AChampionCharacter::ReloadBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AChampionCharacter::Reload);
}

void AChampionCharacter::SetupHealthComponent()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthEmptyEvent.AddUObject(this, &AChampionCharacter::HandleDeath);
}

void AChampionCharacter::HandleDeath()
{
	FString msg = FString::Printf(TEXT("[Client] %s Died"), *GetName());
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, *msg);

	if (OnChampionDeathEvent.IsBound())
	{
		OnChampionDeathEvent.Broadcast();
	}

	//TEMP
	AudioComponent->Multicast_PlayDeathSFX_Implementation();
}

void AChampionCharacter::TryFindWeapon()
{
	HasWeapon = false;

	TArray<AActor*> allChildren;
	GetAllChildActors(allChildren, true);
	for (int i = 0; i < allChildren.Num(); i++)
	{
		ABaseWeapon* tempWeapon = Cast<ABaseWeapon>(allChildren[i]);
		if (tempWeapon != nullptr)
		{
			Weapon = tempWeapon;
			Weapon->SetWeaponOwner(this);
			AnimHandler->BindToWeaponEvents(tempWeapon);
			HasWeapon = true;
		}
	}
}

void AChampionCharacter::ShootingStarted()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Started"));

	if (HasWeapon)
	{
		Weapon->TryUseWeapon();
	}
}

void AChampionCharacter::ShootingStopped()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Stopped"));	
	if (HasWeapon)
	{
		Weapon->StopUsingWeapon();
	}
}

void AChampionCharacter::Reload()
{
	if (HasWeapon)
	{
		ABaseShootingWeapon* shootingWeapon = Cast<ABaseShootingWeapon>(Weapon);
		if (shootingWeapon != nullptr)
		{
			shootingWeapon->TryStartReload();
		}
	}
}

void AChampionCharacter::Server_DoHitScanTrace_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hitscan Shot!"));

	const float lineTraceDistance = 10000.f;
	FHitResult hitResult = FHitResult(ForceInit);
	UCameraComponent* camera = GetFollowCamera();
	check(camera != nullptr);
	FVector cameraLocation = camera->GetComponentLocation();
	float cameraToCharacterDist = (GetActorLocation() - cameraLocation).Size();
	FVector startLocation = cameraLocation + (camera->GetForwardVector() * cameraToCharacterDist);
	FVector endLocation = startLocation + (camera->GetForwardVector() * lineTraceDistance);
	ECollisionChannel channel = ECollisionChannel::ECC_Pawn;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, channel, traceParams);
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Yellow, true, -1, 0, 10.f);
	//DrawDebugSphere(GetWorld(), startLocation, 10.f, 10.f, FColor::Blue, true, 10.f);
	//DrawDebugSphere(GetWorld(), endLocation, 10.f, 10.f, FColor::Red, true, 10.f);

	if (hitResult.IsValidBlockingHit())
	{
		AActor* hitActor = hitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitActor->GetName());
		UHealthComponent* hpComponent = Cast<UHealthComponent>(hitActor->GetComponentByClass(UHealthComponent::StaticClass()));
		if (hpComponent != nullptr)
		{
			hpComponent->ApplyDamage(10.f);
		}
	}
}

//TODO: Move to a VFX component do an object pool.
void AChampionCharacter::Multicast_PlayShotVFX_Implementation()
{
	//TODO: This is very bad I know. Its just for testing.
	UParticleSystem* newParticle = NewObject<UParticleSystem>();
}


