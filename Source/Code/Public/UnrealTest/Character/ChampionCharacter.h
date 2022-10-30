// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "ChampionCharacter.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class UNREALTEST_API AChampionCharacter : public AUnrealTestCharacter
{
	GENERATED_BODY()

	AChampionCharacter();
	
	class UHealthComponent* HealthComponent = nullptr;
	class UChampionAnimHandlerComp* AnimHandler = nullptr;

public :
	FORCEINLINE const UHealthComponent* GetHealthComponentPtr() const { return HealthComponent; }
	FORCEINLINE UHealthComponent* AccessHealthComponentPtr() { return HealthComponent; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void SetupHealthComponent();

	UFUNCTION(Client, Reliable)
	void Client_HandleDeath();

	UFUNCTION()
	void ShootingStarted();

	UFUNCTION()
	void ShootingStopped();

	UFUNCTION(Server, Reliable)
	void Server_DoHitScanTrace();

};
