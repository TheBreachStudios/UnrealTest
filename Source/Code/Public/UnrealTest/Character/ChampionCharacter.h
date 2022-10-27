// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "ChampionCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AChampionCharacter : public AUnrealTestCharacter
{
	GENERATED_BODY()

	class UHealthComponent* HealthComponent = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void BindHealthEvents();

	UFUNCTION()
	void HandleDeath();

	void ShootingStarted();
	void ShootingStopped();
	FHitResult HitScanTrace();
};
