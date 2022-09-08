#include "UnrealTest/Character/Hero/UnrealTestHeroBase.h"
#include "UnrealTest/Weapon/UnrealTestWeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Math/UnrealMathUtility.h"

AUnrealTestHeroBase::AUnrealTestHeroBase()
{
	CurrentHealth = MaxHealth;
}

void AUnrealTestHeroBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void AUnrealTestHeroBase::UseWeapon()
{
	Server_UseWeapon();
}

void AUnrealTestHeroBase::SetTeamColor(int NewIndex)
{
	ServerSetTeamColor(NewIndex);
}

void AUnrealTestHeroBase::Server_UseWeapon_Implementation()
{
	Weapon->Use();
}

bool AUnrealTestHeroBase::Server_UseWeapon_Validate()
{
	return true;
}

void AUnrealTestHeroBase::ModifyCurrentHealth(float Amount)
{
	CurrentHealth += Amount;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth; 
	}

	if (CurrentHealth <= 0)
	{
		Die();
	}
}

float AUnrealTestHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ModifyCurrentHealth(-DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AUnrealTestHeroBase::Die()
{
	UE_LOG(LogTemp, Display, TEXT("A hero has died"));
	Respawn();
}

void AUnrealTestHeroBase::Respawn()
{
	UE_LOG(LogTemp, Display, TEXT("%s called"), ANSI_TO_TCHAR(__FUNCTION__));
	CurrentHealth = MaxHealth;

	TArray<AActor*> FoundPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPlayerStarts);

	ensureMsgf(FoundPlayerStarts.Num() > 0, TEXT("%s: Trying to respawn hero, but could not find any PlayerStarts in the scene"), ANSI_TO_TCHAR(__FUNCTION__));

	AActor* RandomPlayerStart = FoundPlayerStarts[FMath::RandRange(0, FoundPlayerStarts.Num() - 1)];
	this->SetActorLocation(RandomPlayerStart->GetActorLocation());

}

void AUnrealTestHeroBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnrealTestHeroBase, TeamIndex);
}

void AUnrealTestHeroBase::ServerSetTeamColor_Implementation(int NewIndex)
{
	TeamIndex = NewIndex;

	if (TeamIndex > 0)
	{
		GetMesh()->SetMaterial(1, BlueTeamMaterial);
	}
	else
	{
		GetMesh()->SetMaterial(1, RedTeamMaterial);
	}
}

bool AUnrealTestHeroBase::ServerSetTeamColor_Validate(int NewIndex)
{
	return true;
}

void AUnrealTestHeroBase::OnRep_TeamIndex()
{
	if (TeamIndex > 0)
	{
		GetMesh()->SetMaterial(1, BlueTeamMaterial);
	}
	else
	{
		GetMesh()->SetMaterial(1, RedTeamMaterial);
	}
}
