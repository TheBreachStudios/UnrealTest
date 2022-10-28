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

	AChampionCharacter();
	
	class UHealthComponent* HealthComponent = nullptr;

public :
	FORCEINLINE const UHealthComponent* GetHealthComponentPtr() const { return HealthComponent; }
	FORCEINLINE UHealthComponent* AccessHealthComponentPtr() { return HealthComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void BindHealthEvents();

	UFUNCTION()
	void HandleDeath();

	UFUNCTION(Server, Reliable)
	void Server_ShootingStarted();

	void ShootingStopped();
};
