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

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateMovementSpeed();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateSidewaysMovementDirection();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateIsInAir();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetIsDead();

	void BindHealthEvents();	

	AActor* OwningActor = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	float MovementSpeed = 0.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float SidewaysMovementDirection = 0.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsInAir = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsDead = false;
	
};
