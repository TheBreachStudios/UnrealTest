// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealTestCharacter.generated.h"

class UUT_HealthComponent;

UCLASS(config=Game)
class AUnrealTestCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//Health Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UUT_HealthComponent* HealthComponent;

	//Time To respawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Death, meta = (AllowPrivateAccess = "true"))
	float TimeToRespawn;

public:
	AUnrealTestCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void RespawnCharacter();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void DisableCotrollerRotation();
	void ConfigureCharacterMovement(class UCharacterMovementComponent* characterMovement);
	void SetCameraBoom();
	void SetFollowCamera();

	void JumpBinding(class UInputComponent* PlayerInputComponent);
	void MovementBinding(class UInputComponent* PlayerInputComponent);
	void TurnBinding(class UInputComponent* PlayerInputComponent);
	void LookUpBinding(class UInputComponent* PlayerInputComponent);
	void TouchBinding(class UInputComponent* PlayerInputComponent);

	const float TURN_RATE_GAMEPAD = 50.f;
	const float JUMP_Z_VELOCITY= 700.f;
	const float AIR_CONTROL = 0.35f;
	const float MAX_WALK_SPEED = 500.f;
	const float MIN_ANALOG_WALK_SPEED = 20.f;
	const float BRAKING_DECELERATION_WALKING = 2000.f;

	//Getter of the Health Component
	UFUNCTION(BlueprintCallable)
	UUT_HealthComponent* GetHealthComponent() const;

	void SetHealthComponent();

	//Die Function called when health component send the event
	UFUNCTION()
	void Die(AActor* ActorToDie);


private:
	//Functions tp Enable/Disable Capsule collision
	void EnableCapsuleCollision();
	void DisableCapsuleCollision();

	//Functions to Enable/Disable movement
	void EnableMovement();
	void DisableMovement();

	//Functions to Enable/Disable Input
	void EnablePlayerInput();
	void DisablePlayerInput();

	//Do Ragdoll Anim
	void ApplyRagdoll();
	//Reattach Ragdoll to capsule
	void ReAttachRagdoll();

};

