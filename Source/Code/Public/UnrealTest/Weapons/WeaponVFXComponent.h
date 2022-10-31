// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponVFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UWeaponVFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponVFXComponent();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackVFX();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetVFX();

protected:
	virtual void BeginPlay() override;	

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ShotVFX = nullptr;

	class UStaticMeshComponent* WeaponMeshComponent = nullptr;

	const FName MUZZLE_SOCKET_NAME = FName("Muzzle");
};
