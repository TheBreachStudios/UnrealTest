// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "UnrealTest/Weapons/WeaponComponent.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealTestCharacter

AUnrealTestCharacter::AUnrealTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = TURN_RATE_GAMEPAD;

	DisableCotrollerRotation();

	ConfigureCharacterMovement(GetCharacterMovement());
	
	SetCameraBoom();
	SetFollowCamera();

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	bReplicates = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AUnrealTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	// to prevent the Unreal network from playing tricks on us we are going to disable replication

	if(Cast<APlayerController>(Controller))
		Cast<APlayerController>(Controller)->SetInputMode(FInputModeGameAndUI());
	
}


void AUnrealTestCharacter::DisableCotrollerRotation()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// In this case we implement a simple health deduction using SetCurrentHealth for takedamage
float AUnrealTestCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	float currentHealth = HealthComponent->GetCurrentHealth();
	float damageApplied = currentHealth - DamageTaken;
	HealthComponent->SetCurrentHealth(damageApplied);
	return damageApplied;
}

void AUnrealTestCharacter::ConfigureCharacterMovement(UCharacterMovementComponent* characterMovement)
{
	// Configure character movement
	characterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	characterMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	characterMovement->JumpZVelocity = JUMP_Z_VELOCITY;
	characterMovement->AirControl = AIR_CONTROL;
	characterMovement->MaxWalkSpeed = MAX_WALK_SPEED;
	characterMovement->MinAnalogWalkSpeed = MIN_ANALOG_WALK_SPEED;
	characterMovement->BrakingDecelerationWalking = BRAKING_DECELERATION_WALKING;
}

void AUnrealTestCharacter::SetCameraBoom()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
}

void AUnrealTestCharacter::SetFollowCamera()
{
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	JumpBinding(PlayerInputComponent);

	MovementBinding(PlayerInputComponent);

	TurnBinding(PlayerInputComponent);
	LookUpBinding(PlayerInputComponent);
	
	TouchBinding(PlayerInputComponent);

	ShootBinding(PlayerInputComponent);
}

void AUnrealTestCharacter::JumpBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AUnrealTestCharacter::MovementBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AUnrealTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AUnrealTestCharacter::MoveRight);
}

void AUnrealTestCharacter::TurnBinding(class UInputComponent* PlayerInputComponent)
{
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AUnrealTestCharacter::TurnAtRate);
}

void AUnrealTestCharacter::LookUpBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AUnrealTestCharacter::LookUpAtRate);
}

void AUnrealTestCharacter::TouchBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealTestCharacter::TouchStopped);
}

//To shoot the current weapon with the binding key for shoot (left mouse button)
void AUnrealTestCharacter::ShootBinding(class UInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AUnrealTestCharacter::ShootWeapon);
}


void AUnrealTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AUnrealTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}


void AUnrealTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AUnrealTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AUnrealTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUnrealTestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUnrealTestCharacter::ShootWeapon()
{
	// Shoot the current weapon
	WeaponComponent->StartFire();
}
