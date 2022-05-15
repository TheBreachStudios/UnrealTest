// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FindSessionsCallbackProxy.h"

// Game Project
#include "UnrealTestGameInstanceSubsystem.generated.h"

// Definitions
UENUM(BlueprintType)
enum class EBPOnJoinSessionCompleteResult : uint8
{
	/** The join worked as expected */
	Success,
	/** There are no open slots to join */
	SessionIsFull,
	/** The session couldn't be found on the service */
	SessionDoesNotExist,
	/** There was an error getting the session server's address */
	CouldNotRetrieveAddress,
	/** The user attempting to join is already a member of the session */
	AlreadyInSession,
	/** An error not covered above occurred */
	UnknownError
};

/**
 * 
 */
UCLASS()
class UNREALTEST_API UUnrealTestGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Delegates
public:
	// On session creation completed delegate
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionCompleted, bool, Successful);
	UPROPERTY(BlueprintAssignable, Category = "SessionHandling")
	FOnCreateSessionCompleted OnCreateSessionCompleted;

	// On session start completed delegate
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartSessionCompleted, bool, Successful);
	UPROPERTY(BlueprintAssignable, Category = "SessionHandling")
	FOnStartSessionCompleted OnStartSessionCompleted;

	// On session find completed delegate
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSessionsCompleted, const TArray<FBlueprintSessionResult>&, SessionResults, bool, Successful);
	UPROPERTY(BlueprintAssignable, Category = "SessionHandling")
	FOnFindSessionsCompleted OnFindSessionsCompleted;

	// On session joining completed delegate
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinSessionCompleted, EBPOnJoinSessionCompleteResult, Result);
	UPROPERTY(BlueprintAssignable, Category = "SessionHandling")
	FOnJoinSessionCompleted OnJoinGameSessionCompleted;

private:
	// Internal on session creation completed delegate
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	// Internal on session creation completed delegate handle
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	// Internal on session start completed delegate
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	// Internal on session start completed delegate handle
	FDelegateHandle StartSessionCompleteDelegateHandle;

	// Internal on session find completed delegate
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	// Internal on session find completed delegate handle
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	// Internal on session join completed delegate 
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	// Internal on session join completed delegate handle
	FDelegateHandle JoinSessionCompleteDelegateHandle;
#pragma endregion Delegates

#pragma region Variables
	// Variables
private:
	// Last session settings
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	// Last session search
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
#pragma endregion Variables

#pragma region Initialization
//Initialization
public:
	// Constructor
	UUnrealTestGameInstanceSubsystem();
#pragma endregion Initialization

#pragma region Functions
private:
	EOnJoinSessionCompleteResult::Type TranslateToCPPOnJoinSessionCompleteResult(EBPOnJoinSessionCompleteResult JoinResult);

	EBPOnJoinSessionCompleteResult TranslateToBPOnJoinSessionCompleteResult(EOnJoinSessionCompleteResult::Type JoinResult);

protected:
	// On create session completed event
	UFUNCTION()
	void OnCreateSessionCompletedEvent(FName SessionName, bool Successful);

	// On session start completed event
	UFUNCTION()
	void OnStartSessionCompletedEvent(FName SessionName, bool Successful);

	// On session find completed event
	UFUNCTION()
	void OnFindSessionsCompletedEvent(bool Successful);

	// On session join completed event
	void OnJoinSessionCompletedEvent(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Add player to session


public:
	// Create game session
	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 MaxPublicConections, bool IsLANMatch);

	// Start game session
	UFUNCTION(BlueprintCallable)
	void StartSession();

	// Find game session
	UFUNCTION(BlueprintCallable)
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);

	// Join game session
	UFUNCTION(BlueprintCallable)
	void JoinGameSession(const FBlueprintSessionResult& SessionResultStruct);

	// Try travel to current session
	UFUNCTION(BlueprintCallable)
	bool TryTravelToCurrentSession();
#pragma endregion Functions
};
