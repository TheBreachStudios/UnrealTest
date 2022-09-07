#pragma once

#include "CoreMinimal.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTestHeroBase.generated.h"

class AUnrealTestWeaponBase;

/**
 * Abstract base class parent to all heroes. Contains common logic to heroes.
 */
UCLASS(Abstract)
class UNREALTEST_API AUnrealTestHeroBase : public AUnrealTestCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamIndex)
	int TeamIndex = 0;

public:
	AUnrealTestHeroBase();

	/** Handle the creation of this hero's weapon */
	virtual void SpawnWeapon() PURE_VIRTUAL(AUnrealTestHeroBase::SpawnWeapon, );

	/** Use this hero's weapon */
	void UseWeapon();

	/** Set team color to this hero */
	void SetTeamColor(int TeamIndex);

	/** Set team color to this hero */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetTeamColor(int NewIndex);

	/** Use this hero's weapon in the server */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UseWeapon();

	/** Get the current health of this hero */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	/** Get the maximum health of this hero */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** Get weapon of this hero*/
	UFUNCTION()
	AUnrealTestWeaponBase* GetWeapon() { return Weapon; }

	/**
	* Modifies this hero's current health by the specified amount
	* @Amount Amount to modify this hero's current health by. Positives numbers will heal and negative number will damage.
	*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ModifyCurrentHealth(float Amount);

	/**
	 * Apply damage to this hero.
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	/**
	* Max health of the hero. Initial value would ideally be specified in a data table to make it more accesible to game designers
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AUnrealTestWeaponBase* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AUnrealTestWeaponBase> DefaultWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team colors");
	UMaterialInterface* BlueTeamMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team colors");
	UMaterialInterface* RedTeamMaterial;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Die();

	UFUNCTION()
	virtual void Respawn();

	UFUNCTION()
	virtual void OnRep_TeamIndex();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Current health of this hero */
	float CurrentHealth;
};
