// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "UnrealTest/Projectile/UnrealTestProjectile.h"

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

	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//Initialize projectile class
	ProjectileClass = AUnrealTestProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AUnrealTestCharacter::DisableCotrollerRotation()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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

	ActionBinding(PlayerInputComponent);
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

void AUnrealTestCharacter::ActionBinding(class UInputComponent* PlayerInputComponent)
{
	// Handle firing projectiles
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnrealTestCharacter::StartFire);
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

//////////////////////////////////////////////////////////////////////////
// Replicated Properties

void AUnrealTestCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(AUnrealTestCharacter, CurrentHealth);
}

void AUnrealTestCharacter::OnHealthUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//Functions that occur on all machines. 
	/*
		Any special functionality that should occur as a result of damage or death should be placed here.
	*/
}

void AUnrealTestCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AUnrealTestCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float AUnrealTestCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

void AUnrealTestCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AUnrealTestCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void AUnrealTestCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void AUnrealTestCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AUnrealTestProjectile* spawnedProjectile = GetWorld()->SpawnActor<AUnrealTestProjectile>(spawnLocation, spawnRotation, spawnParameters);
}