#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "UnrealTestGameInstance.generated.h"

/**
 * Custom game instance class. Handle game session logic
 */
UCLASS()
class UNREALTEST_API UUnrealTestGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UUnrealTestGameInstance(const FObjectInitializer& ObjectInitializer);

	// Blueprint callable functions ///////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "Network|Sessions")
	void StartOnlineGame();

	UFUNCTION(BlueprintCallable, Category = "Network|Sessions")
	void FindOnlineGames();

	UFUNCTION(BlueprintCallable, Category = "Network|Sessions")
	void JoinOnlineGame();

	UFUNCTION(BlueprintCallable, Category = "Network|Sessions")
	void DestroySessionAndLeaveGame();

	/**
	*	Function to host a game
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/**
	*	Find an online session
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	/**
	*	Joins a session via a search result
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*	@return bool true if successful, false otherwise
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

protected:
	// Delegate functions //////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	*	Function fired when a session create request has completed
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Delegate fired when a session search query has completed
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	/**
	*	Delegate fired when a session join request has completed
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	*	Delegate fired when a destroying an online session has completed
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	virtual FName GenerateRandomSessionName();

protected:
	FName GameSessionName = "GameSession";
	FName SessionMapName = "/Game/ThirdPerson/Maps/ThirdPersonMap";
	int RandomSessionNameLength = 15;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	// Delegates /////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	// Handles ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

};
