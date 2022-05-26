// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UT_SessionSubsystem.generated.h"

/**
 * 
 */

// Own delegate to know if session was correctly created
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUTOnSessionCreatedCompleted, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUTOnSessionUpdatedCompleted, bool, Success);


UCLASS()
class UNREALTEST_API UUT_SessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//Constructor
	UUT_SessionSubsystem();
	
	////////////////////////////////////////////////////////////////////////////
	// CREATE SESSION
	////////////////////////////////////////////////////////////////////////////

	//Creates the session
	void CreateSession(int32 NumPublicConnections, bool IsLAN);
	
	//Event associated to the creation of the session
	FUTOnSessionCreatedCompleted OnCreateSessionCompleteEvent;
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// UPDATE SESSION
	////////////////////////////////////////////////////////////////////////////
	//Updates the session
	void UpdateSession();
	
	//Event associated to the creation of the session
	FUTOnSessionUpdatedCompleted OnSessionUpdatedEvent;
	////////////////////////////////////////////////////////////////////////////


private:
	//Current Session Settings
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	
	//Create Session Delegate And Handle
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	//Own Function to broadcast our event
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

	//Update Sesion Delegates and handles
	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);

};
