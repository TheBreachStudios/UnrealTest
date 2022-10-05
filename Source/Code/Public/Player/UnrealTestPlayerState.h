// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Common/UnrealTestType.h"
#include "AbilitySystemInterface.h"
#include "Player/UnrealTestCharacterListData.h"
#include "Runtime/AIModule/Classes/GenericTeamAgentInterface.h"
#include "UnrealTestPlayerState.generated.h"

class AUnrealTestGameMode;
class AUnrealTestGameState;
class AUnrealTestPlayerTeam;

class UUnrealTestAbilitySystemComponent;
class UUnrealTestAttributeSet;

class UGameplayEffect;

/**
 * Base class for all game player states
 */
UCLASS()
class UNREALTEST_API AUnrealTestPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPhaseDelegate, EPlayerPhase, NewState);
	UPROPERTY(BlueprintAssignable)
	FPlayerPhaseDelegate OnPlayerPhaseChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectCharacter, int32, CharacterIndex);
	UPROPERTY(BlueprintAssignable)
	FSelectCharacter OnSelectCharacter;

	AUnrealTestPlayerTeam* GetPlayerTeam() const;
	void SetPlayerTeam(AUnrealTestPlayerTeam* Team);

	UFUNCTION(BlueprintCallable)
	EPlayerPhase GetPlayerPhase() const { return PlayerPhase; }
	void SetPlayerPhase(EPlayerPhase NewPhase);

	bool CanRespawn() const;

	void SetTimeOfDeath(float Time);
	UFUNCTION(BlueprintCallable)
	float GetRespawnTimer() const;

	void AddKillCount(int32 Count = 1);
	void SetKillCount(int32 Count);
	int32 GetKillCount() const;
	
	void AddDeathCount(int32 Count = 1);
	void SetDeathCount(int32 Count);
	int32 GetDeathCount() const;

	void AddAssistCount(int32 Count = 1);
	void SetAssistCount(int32 Count);
	int32 GetAssistCount() const;

	UFUNCTION(BlueprintPure, Category = "PlayerState")
	FPlayerStatistic GetPlayerStatistic();

	UFUNCTION(BlueprintPure, Category = "PlayerState")
	FGenericTeamId GetTeamID() const;

	TSubclassOf<ACharacter> GetSelectedPawnClass() const { return SelectedPlayerPawn; }

	void SetPlayerPawn(TSubclassOf<ACharacter> NewClass);
	void SetPlayerPawn(int32 CharacterIndex);

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	UUnrealTestAbilitySystemComponent* GetKTAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetPlayerAvatar(TSubclassOf<APawn> AvatarClass);
	void SetPlayerAvatar(int32 AvatarIndex);
	TSubclassOf<AUnrealTestCharacter> GetPlayerAvatar() const;
	int32 GetPlayerAvatarIndex() const;

	//Kills Count
	TMap<AUnrealTestPlayerState*, int32> PlayerKillStat;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> CharacterStatEffects;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_PlayerPhase)
	EPlayerPhase PlayerPhase;

	UPROPERTY(BlueprintReadOnly, Replicated)
	AUnrealTestPlayerTeam* PlayerTeam;

	AUnrealTestPlayerState();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void OnRep_PlayerPhase();
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_PlayerStatistic();

	UFUNCTION(BlueprintCallable)
	AUnrealTestGameState* GetUTGameState() const;

	UFUNCTION(BlueprintCallable)
	AUnrealTestGameMode* GetUTGameMode() const;

	UFUNCTION(BlueprintCallable)
	AController* GetController() const;

	const UUnrealTestAttributeSet* GetAttributeSet() const;

protected:
	//For UI
	UPROPERTY(Transient, BlueprintReadOnly)
	UUnrealTestCharacterListData* CharactersList;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SelectedCharacterIndex)
	int32 SelectedCharacterIndex = -1;

	UFUNCTION()
	virtual void OnRep_SelectedCharacterIndex();

private:
	bool IsMatchActive() const;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Param")
	TSubclassOf<ACharacter> SelectedPlayerPawn;

	UPROPERTY(Replicated)
	float TimeOfDeath = 0.f;

	//Statistics
	UPROPERTY(Transient, ReplicatedUsing = OnRep_PlayerStatistic)
	int32 KillCount;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_PlayerStatistic)
	int32 DeathCount;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_PlayerStatistic)
	int32 AssistCount;

	UUnrealTestAbilitySystemComponent* AbilitySystemComponent;
	mutable const UUnrealTestAttributeSet* AttributeSet;

	mutable TWeakObjectPtr<AUnrealTestGameState> GameStatePtr;
	mutable TWeakObjectPtr<AUnrealTestGameMode> GameModePtr;
	mutable TWeakObjectPtr<AController> ControllerPtr;
};
