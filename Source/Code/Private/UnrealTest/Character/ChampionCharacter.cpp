// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Camera/CameraComponent.h"

AChampionCharacter::AChampionCharacter()
{
	HealthComponent = nullptr;
}

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	verify((HealthComponent = Cast<UHealthComponent>(FindComponentByClass(UHealthComponent::StaticClass()))) != nullptr);
	
	BindHealthEvents();
}

void AChampionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ShootBinding(PlayerInputComponent);
}

void AChampionCharacter::ShootBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AChampionCharacter::Server_ShootingStarted);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AChampionCharacter::ShootingStopped);
}

void AChampionCharacter::BindHealthEvents()
{
	if (HealthComponent == nullptr) { return; }

	HealthComponent->OnHealthEmpty.AddUObject(this, &AChampionCharacter::HandleDeath);
}

void AChampionCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Died"), *GetName());
}

void AChampionCharacter::Server_ShootingStarted_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting Started"));

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

void AChampionCharacter::ShootingStopped()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Stopped"));	
}
