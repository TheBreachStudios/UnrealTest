// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionClient.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/OnlineReplStructs.h"
#include "UObject/UObjectGlobals.h"
#include "Online/CoreOnline.h"
#include "IImageWrapperModule.h"
#include "UnrealTestGameInstance.generated.h"





/**
 * 
 */
UCLASS()
class UNREALTEST_API UUnrealTestGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UUnrealTestGameInstance(const FObjectInitializer& ObjectInitializer);
	/*
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/**
*	Delegate fired when a session search query has completed
*
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	void OnFindSessionsComplete(bool bWasSuccessful);


	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);


	/*
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);


	/*
	*	Function fired when a session create request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/*
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	/*
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/*
*	Delegate fired when a session join request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	TSharedPtr<class FOnlineSessionSettings> SessionSettings;


	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/* Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/* Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/* Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/* Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/* Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/* Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/* Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;




	/* FOR USE IN BUTTONS IN THE FIRST MENU INGAME IN THE CLIENT THAT WANTS TO BE HOST
	Callable from blueprints to create sessions match*/
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void StartOnlineGame();

	/* FOR USE IN BUTTONS IN THE FIRST MENU INGAME IN THE CLIENT THAT WANTS HOSTED GAMES LIST
	Callable from blueprints to list the created sessions match*/
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void FindOnlineGames();
	
	/* FOR USE IN BUTTONS IN THE FIRST MENU INGAME IN THE CLIENT THAT WANTS TO JOIN A HOSTED MATCH
	Callable from blueprints to join sessions match*/
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void JoinOnlineGame();

	/*DESTROYING MATCH SESSION FROM BLUEPRINTS*/
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void DestroySessionAndLeaveGame();

};
