// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "Camera/CameraComponent.h"

AChampionCharacter::AChampionCharacter()
{
	HealthComponent = nullptr;
	bReplicates = true;	
	bAlwaysRelevant = true;

	SetupHealthComponent();
	AnimHandler = CreateDefaultSubobject<UChampionAnimHandlerComp>(TEXT("AnimationHandlerComponent"));
}

void AChampionCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PlayerController %s has possessed %s"), *NewController->GetName(), *GetName());
}

void AChampionCharacter::UnPossessed()
{
	Super::UnPossessed();
	UE_LOG(LogTemp, Warning, TEXT("Champion Unpossessed!"));
}

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Champion Begin Play!"));

	//APlayerController* playerController = Cast<APlayerController>(GetController());
	//if (playerController != nullptr)
	//{
	//	EnableInput(playerController);
	//}
}

void AChampionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ShootBinding(PlayerInputComponent);
}

void AChampionCharacter::ShootBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AChampionCharacter::ShootingStarted);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AChampionCharacter::ShootingStopped);
}

void AChampionCharacter::SetupHealthComponent()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthEmptyEvent.AddUObject(this, &AChampionCharacter::Client_HandleDeath);
}

void AChampionCharacter::Client_HandleDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Died"), *GetName());
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController != nullptr) 
	{
		DisableInput(playerController);
	}
}


void AChampionCharacter::ShootingStarted()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Started"));

	Server_DoHitScanTrace();
}

void AChampionCharacter::ShootingStopped()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Stopped"));	
}

void AChampionCharacter::Server_DoHitScanTrace_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hitscan Shot!"));

	const float lineTraceDistance = 1000.f;
	FHitResult hitResult = FHitResult(ForceInit);
	UCameraComponent* camera = GetFollowCamera();
	check(camera != nullptr);
	FVector startLocation = camera->GetComponentLocation();
	FVector endLocation = camera->GetComponentRotation().Vector() * lineTraceDistance;
	ECollisionChannel channel = ECollisionChannel::ECC_Pawn;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, channel, traceParams);

	if (hitResult.IsValidBlockingHit())
	{
		AActor* hitActor = hitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitActor->GetName());
		UHealthComponent* hpComponent = Cast<UHealthComponent>(hitActor->GetComponentByClass(UHealthComponent::StaticClass()));
		if (hpComponent != nullptr)
		{
			hpComponent->ApplyDamage(10.f);
		}
	}
}


