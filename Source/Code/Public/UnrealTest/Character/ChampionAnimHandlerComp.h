// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChampionAnimHandlerComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UChampionAnimHandlerComp : public UActorComponent
{
	GENERATED_BODY()

	

public:	
	UChampionAnimHandlerComp();

	void UpdateMovementSpeed();
	void UpdateSidewaysMovementDirection();
	void UpdateIsInAir();

	void BindHealthEvents();

	UFUNCTION()
	void SetIsDead();

	AActor* OwningActor = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float SidewaysMovementDirection = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool IsInAir = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsDead = false;
	
};
