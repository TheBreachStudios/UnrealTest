// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "Camera/CameraComponent.h"

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = Cast<UHealthComponent>(FindComponentByClass(UHealthComponent::StaticClass()));
	check(HealthComponent);
	BindHealthEvents();
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

void AChampionCharacter::BindHealthEvents()
{
	if (HealthComponent == nullptr) { return; }

	HealthComponent->OnHealthEmpty.AddUObject(this, &AChampionCharacter::HandleDeath);
}

void AChampionCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("I Died"));
}

void AChampionCharacter::ShootingStarted()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting Started"));

	FHitResult hit = HitScanTrace();
	if (hit.IsValidBlockingHit())
	{
		AActor* hitActor = hit.GetActor();
		FString hitActorName = hitActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitActorName);
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

FHitResult AChampionCharacter::HitScanTrace()
{
	const float lineTraceDistance = 1000.f;
	FHitResult hitResult = FHitResult(ForceInit);
	UCameraComponent* camera = GetFollowCamera();
	check(camera);
	FVector startLocation = camera->GetComponentLocation();
	FVector endLocation = camera->GetComponentRotation().Vector() * lineTraceDistance;
	ECollisionChannel channel = ECollisionChannel::ECC_Pawn;//TEMP
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, channel, traceParams);
	return hitResult;
}
