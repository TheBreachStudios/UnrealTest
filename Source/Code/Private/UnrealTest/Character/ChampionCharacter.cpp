// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

AChampionCharacter::AChampionCharacter()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	SetupHealthComponent();
	AnimHandler = CreateDefaultSubobject<UChampionAnimHandlerComp>(TEXT("AnimationHandlerComponent"));
}

void AChampionCharacter::Multicast_ResetChampionCharacter_Implementation()
{
	HealthComponent->ResetCurrentHealth();
	AnimHandler->Multicast_ResetAnimation();
}

void AChampionCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Champion Begin Play!"));
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
	HealthComponent->OnHealthEmptyEvent.AddUObject(this, &AChampionCharacter::HandleDeath);
}

void AChampionCharacter::HandleDeath()
{
	FString msg = TEXT("[Client] %s Died"), * GetName();
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, *msg);

	if (OnChampionDeathEvent.IsBound())
	{
		OnChampionDeathEvent.Broadcast();
	}
}

void AChampionCharacter::ShootingStarted()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Started"));

	Server_DoHitScanTrace();
	Multicast_DebugHitScanTrace();
}

void AChampionCharacter::ShootingStopped()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Stopped"));	
}

void AChampionCharacter::Server_DoHitScanTrace_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hitscan Shot!"));

	const float lineTraceDistance = 10000.f;
	FHitResult hitResult = FHitResult(ForceInit);
	UCameraComponent* camera = GetFollowCamera();
	check(camera != nullptr);
	FVector cameraLocation = camera->GetComponentLocation();
	float cameraToCharacterDist = (GetActorLocation() - cameraLocation).Size();
	FVector startLocation = cameraLocation + (camera->GetForwardVector() * cameraToCharacterDist);
	FVector endLocation = startLocation + (camera->GetForwardVector() * lineTraceDistance);
	ECollisionChannel channel = ECollisionChannel::ECC_Pawn;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, channel, traceParams);
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Yellow, true, -1, 0, 10.f);
	//DrawDebugSphere(GetWorld(), startLocation, 10.f, 10.f, FColor::Blue, true, 10.f);
	//DrawDebugSphere(GetWorld(), endLocation, 10.f, 10.f, FColor::Red, true, 10.f);

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

void AChampionCharacter::Multicast_DebugHitScanTrace_Implementation()
{
	const float lineTraceDistance = 10000.f;
	UCameraComponent* camera = GetFollowCamera();
	check(camera != nullptr);
	FVector cameraLocation = camera->GetComponentLocation();
	float cameraToCharacterDist = (GetActorLocation() - cameraLocation).Size();
	FVector startLocation = cameraLocation + (camera->GetForwardVector() * cameraToCharacterDist);
	FVector endLocation = startLocation + (camera->GetForwardVector() * lineTraceDistance);
	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Yellow, true, -1, 0, 10.f);
	DrawDebugSphere(GetWorld(), startLocation, 10.f, 10.f, FColor::Blue, true, 10.f);
	DrawDebugSphere(GetWorld(), endLocation, 10.f, 10.f, FColor::Red, true, 10.f);
}


