// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Game Project
#include "UnrealTestCharacter.generated.h"

// Definitions
class ABaseWeapon;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class UInputComponent;
class UHealthComponent;
class UWidgetComponent;
class UHealthBarWidget;
class AUnrealTestHUD;

UCLASS(config=Game)
class AUnrealTestCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Configuration
// Configuration
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float TurnRateGamepad;

	// Weapon that will be spawn with the character
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<ABaseWeapon> InitialWeaponTemplate;

	// Weapon that will be spawn with the character
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	// Weapon that will be spawn with the character
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	TSubclassOf<UHealthBarWidget> HealthWidgetWorldSpaceTemplate;
#pragma endregion Configuration

#pragma region Variables
// Variables
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	// TODO: Once animations has been implemented removed this and attach weapo to hand sokket
	// Scene component in which to attach the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponHolder = nullptr;

	// TODO: Once animations has been implemented removed this and attach weapo to hand sokket
	// Scene component in which to attach the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	ABaseWeapon* CurrentWeapon = nullptr;

	// Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	// Widget component for health display on simulated players
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthWidgetComponent = nullptr;
	
	// HUD reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	AUnrealTestHUD* PlayerHUD = nullptr;

public:
	const float TURN_RATE_GAMEPAD = 50.f;
	const float JUMP_Z_VELOCITY = 700.f;
	const float AIR_CONTROL = 0.35f;
	const float MAX_WALK_SPEED = 500.f;
	const float MIN_ANALOG_WALK_SPEED = 20.f;
	const float BRAKING_DECELERATION_WALKING = 2000.f;
#pragma endregion Variables

#pragma region Initialization
// Initialization
public:
	// Constructor
	AUnrealTestCharacter();
#pragma endregion Initialization

#pragma region Getters / Setters
// Getters / Setters
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Set Camera BoomConstructor
	void SetCameraBoom();

	// Set Follow Camera
	void SetFollowCamera();

	// Set Weapon Holder
	void SetWeaponHolder();

	// Set Health Component
	void SetHealthComponent();

	// Get Health Component
	UHealthComponent* GetHealthComponent() { return HealthComponent; };
#pragma endregion Getters / Setters

#pragma region Overrides
// Overrides
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#pragma endregion Overrides

#pragma region Functions
// Functions
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

	// Start shooting
	void Shoot();

	// Start shooting
	void StopShoot();

	// Server shoot handle
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Shoot();
	void Server_Shoot_Implementation();
	bool Server_Shoot_Validate();

	// Call game over event
	// Server shoot handle
	UFUNCTION(Client, Reliable, WithValidation)
	void Client_GameOver(int32 DefeatedTeamID);
	void Client_GameOver_Implementation(int32 DefeatedTeamID);
	bool Client_GameOver_Validate(int32 DefeatedTeamID);

public:
	// Initialize weapons values
	void InitiliazeCharacter();

	// Disable controller rotation
	void DisableCotrollerRotation();

	// Set up character movement component
	void ConfigureCharacterMovement(UCharacterMovementComponent* characterMovement);

	// Binds jump inputs
	void JumpBinding(UInputComponent* PlayerInputComponent);
	
	// Binds movement inputs
	void MovementBinding(UInputComponent* PlayerInputComponent);

	// Binds shoot inputs
	void ShootBinding(UInputComponent* PlayerInputComponent);

	// Binds turning inputs
	void TurnBinding(UInputComponent* PlayerInputComponent);

	// Binds aiming inputs
	void LookUpBinding(UInputComponent* PlayerInputComponent);

	// TODO: For now it will just restore health to max
	// Respawn player
	void RespawnPlayer();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_Die();
	void Multicast_Die_Implementation();
	bool Multicast_Die_Validate();

#pragma endregion Functions

};

