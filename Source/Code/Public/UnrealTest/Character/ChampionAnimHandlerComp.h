// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChampionAnimHandlerComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UChampionAnimHandlerComp : public UActorComponent
{
	GENERATED_BODY()

	//UFUNCTION(Client, Unreliable)
	//void Client_PrintPropertyValues() const;

public:	
	UChampionAnimHandlerComp();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateMovementProperties();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetIsDead();
	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_UpdateIsShooting();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetStartedReloading();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetEndedReloading();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetAnimation();

	void BindToHealthEvents();
	void BindToWeaponEvents(class ABaseWeapon* weapon);


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	void SetUpperBodyBlending(bool setActive);

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	float MovementSpeed = 0.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float SidewaysMovementDirection = 0.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float UpperBodyBlending = 0.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsInAir = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsDead = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsReloading = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsShooting = false;
	
};
