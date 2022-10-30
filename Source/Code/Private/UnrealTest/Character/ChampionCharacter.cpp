// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ChampionCharacter.h"
#include "UnrealTest/Character/HealthComponent.h"
#include "UnrealTest/Character/ChampionAnimHandlerComp.h"
#include "Camera/CameraComponent.h"

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
	FString messageStr = FString::Printf(TEXT("[Client] %s Died"), *GetName());
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, messageStr);

	if (OnChampionDeathEvent.IsBound())
	{
		OnChampionDeathEvent.Broadcast();
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


