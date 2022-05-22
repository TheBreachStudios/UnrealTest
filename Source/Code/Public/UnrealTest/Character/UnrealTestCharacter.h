// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

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
	// Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration|Input")
	float TurnRateGamepad;

	// Weapon class that will be spawn with the character.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Weapons")
	TSubclassOf<ABaseWeapon> InitialWeaponTemplate = nullptr;

	// Max health value.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Health")
	float MaxHealth = 100.f;

	// Healthbar widget class to be created over non-local players.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Health")
	TSubclassOf<UHealthBarWidget> HealthWidgetWorldSpaceTemplate = nullptr;
#pragma endregion Configuration

#pragma region Variables
// Variables
private:
	// Camera boom positioning the camera behind the character. 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	// Follow camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	// TODO: Once animations has been implemented removed this and attach weapon to hand socket.
	// Scene component in which to attach the weapon.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponHolder = nullptr;

	// Current weapon reference.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	ABaseWeapon* CurrentWeapon = nullptr;

	// Health component refere.ce.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	// Widget component for health display on simulated players reference.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthWidgetComponent = nullptr;
	
	// HUD reference.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	AUnrealTestHUD* PlayerHUD = nullptr;

public:
	// Gamepad trun rate.
	const float TURN_RATE_GAMEPAD = 50.f;

	// Z velocity on jump.
	const float JUMP_Z_VELOCITY = 700.f;

	// Air control factor.
	const float AIR_CONTROL = 0.35f;
	
	// Waximum walk speed.
	const float MAX_WALK_SPEED = 500.f;
	
	// Minimum analog walk speed.
	const float MIN_ANALOG_WALK_SPEED = 20.f;
	
	// Braking deceleration on walking.
	const float BRAKING_DECELERATION_WALKING = 2000.f;
#pragma endregion Variables

#pragma region Initialization
// Initialization
public:
	// Constructor.
	AUnrealTestCharacter();

	// Initialize Camera Boom.
	void InitializeCameraBoom();

	// Initialize Follow Camera.
	void InitializeFollowCamera();

	// Initialize Weapon Holder.
	void InitializeWeaponHolder();

	// Initialize Health Component.
	void InitializeHealthComponent();
#pragma endregion Initialization

#pragma region Getters / Setters
// Getters / Setters
public:
	// Returns CameraBoom subobject.
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera subobject.
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Get Health Component.
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; };
#pragma endregion Getters / Setters

#pragma region Overrides
// Overrides
protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Binds inputs.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion Overrides

#pragma region Functions
// Functions
protected:

	// Called for forwards/backward input.
	void MoveForward(float Value);

	// Called for side to side input.
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

	// Start shooting.
	void Shoot();

	// Start shooting.
	void StopShoot();

	// Server shoot handle.
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Shoot();
	void Server_Shoot_Implementation();
	bool Server_Shoot_Validate();

	// Call game over event.
	UFUNCTION(Client, Reliable, WithValidation)
	void Client_GameOver(int32 DefeatedTeamID);
	void Client_GameOver_Implementation(int32 DefeatedTeamID);
	bool Client_GameOver_Validate(int32 DefeatedTeamID);

public:
	// Set up character movement component.
	void ConfigureCharacterMovement(UCharacterMovementComponent* characterMovement);

	// Binds jump inputs.
	void JumpBinding(UInputComponent* PlayerInputComponent);
	
	// Binds movement inputs.
	void MovementBinding(UInputComponent* PlayerInputComponent);
	
	// Binds shoot inputs.
	void ShootBinding(UInputComponent* PlayerInputComponent);

	// Binds turning inputs.
	void TurnBinding(UInputComponent* PlayerInputComponent);

	// Binds aiming inputs.
	void LookUpBinding(UInputComponent* PlayerInputComponent);

	// TODO: For now it will just restore health to max
	// Respawn player.
	void RespawnPlayer();

	// Die event.
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_Die();
	void Multicast_Die_Implementation();
	bool Multicast_Die_Validate();

#pragma endregion Functions
};

