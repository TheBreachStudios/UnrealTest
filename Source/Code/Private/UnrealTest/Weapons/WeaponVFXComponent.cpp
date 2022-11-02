// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Weapons/WeaponVFXComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"
#include "Net/UnrealNetwork.h"

UWeaponVFXComponent::UWeaponVFXComponent()
{
	if (ShotVFX == nullptr) 
	{
		static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotVFXClass(TEXT("/Game/VFX/Weapons/P_GunShot"));
		if (ShotVFXClass.Succeeded())
		{
			ShotVFX = ShotVFXClass.Object;
		}
	}
}


void UWeaponVFXComponent::BeginPlay()
{
	Super::BeginPlay();
	WeaponMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	SetIsReplicated(true);
}

void UWeaponVFXComponent::Multicast_PlayAttackVFX_Implementation()
{
	if (GetOwner() != nullptr && WeaponMeshComponent != nullptr && ShotVFX != nullptr)
	{
		UStaticMeshSocket* socket = WeaponMeshComponent->GetStaticMesh()->FindSocket(MUZZLE_SOCKET_NAME);
		if (socket != nullptr) 
		{
			FTransform spawnTransform;
			socket->GetSocketTransform(spawnTransform, WeaponMeshComponent);
			UGameplayStatics::SpawnEmitterAttached(ShotVFX, WeaponMeshComponent, MUZZLE_SOCKET_NAME, spawnTransform.GetLocation(), spawnTransform.Rotator(), EAttachLocation::KeepWorldPosition);
		}
	}
}

void UWeaponVFXComponent::Multicast_ResetVFX_Implementation()
{
	//TODO: Reset all VFX currently playing.
}
