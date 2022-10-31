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
	
	DECLARE_MULTICAST_DELEGATE(FChampionSignature);
	
public :
	AChampionCharacter();

private:

	class UHealthComponent* HealthComponent = nullptr;
	class UChampionAnimHandlerComp* AnimHandler = nullptr;

public:

	FORCEINLINE const UHealthComponent* GetHealthComponentPtr() const { return HealthComponent; }
	FORCEINLINE UHealthComponent* AccessHealthComponentPtr() { return HealthComponent; }

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetChampionCharacter();

	FChampionSignature OnChampionDeathEvent;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootBinding(class UInputComponent* PlayerInputComponent);
	void SetupHealthComponent();

	void HandleDeath();

	UFUNCTION()
	void ShootingStarted();

	UFUNCTION()
	void ShootingStopped();

	UFUNCTION(Server, Reliable)
	void Server_DoHitScanTrace();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DebugHitScanTrace();


};
