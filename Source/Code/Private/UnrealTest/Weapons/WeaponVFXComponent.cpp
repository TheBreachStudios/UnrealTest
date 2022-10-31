// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponVFXComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"

UWeaponVFXComponent::UWeaponVFXComponent()
{
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotVFXClass(TEXT("/Game/VFX/Weapons/P_GunShot"));
	//if (ShotVFXClass.Succeeded())
	//{
	//	ShotVFX = ShotVFXClass.Object;
	//}
}


void UWeaponVFXComponent::BeginPlay()
{
	Super::BeginPlay();
	WeaponMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
}

void UWeaponVFXComponent::Multicast_PlayAttackVFX_Implementation()
{
	if (GetOwner() != nullptr && WeaponMeshComponent != nullptr && ShotVFX != nullptr)
	{
		UStaticMeshSocket* socket = WeaponMeshComponent->GetStaticMesh()->FindSocket(MUZZLE_SOCKET_NAME);
		if (socket != nullptr) 
		{
			FVector actorLocation = GetOwner()->GetActorLocation();
			FVector muzzleRelativeLocation = socket->RelativeLocation;
			FVector spawnLocation = actorLocation + muzzleRelativeLocation;
			FTransform spawnTransform = FTransform(spawnLocation);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotVFX, spawnTransform);
		}
	}
}

void UWeaponVFXComponent::Multicast_ResetVFX_Implementation()
{
	//TODO: Reset all VFX currently playing.
}
