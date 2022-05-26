// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Subsystems/UT_SessionSubsystem.h"
#include "OnlineSubsystemUtils.h"

UUT_SessionSubsystem::UUT_SessionSubsystem()
	//Binds the Onlinesubsystem function of creating the session to our own Delegeta
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted))
	, UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted))
{
}

void UUT_SessionSubsystem::CreateSession(int32 NumPublicConnections, bool IsLAN)
{
	//Get Session
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if(!sessionInterface.IsValid()) 
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Session Interface Not Valid!"));
		//Broadcast Event 
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}
	
	//SetUp Session Settings Struct
	SessionSettings = MakeShareable(new FOnlineSessionSettings);
	SessionSettings->NumPrivateConnections = 0;
	SessionSettings->NumPublicConnections = NumPublicConnections;
	//This by default for the moment allow everything
	SessionSettings->bAllowInvites = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	//Not a dedicated server, using ListenServer for this project
	SessionSettings->bIsDedicated = false;
	SessionSettings->bIsLANMatch = IsLAN;
	SessionSettings->bShouldAdvertise = true;

	SessionSettings->Set(SETTING_MAPNAME, FString("MapName"), EOnlineDataAdvertisementType::ViaOnlineService);
	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	//Create Session from the current Player
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings))
	{
		//Clear our handle and broadcast error
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UUT_SessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		//Vlear Delegate Handle
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void UUT_SessionSubsystem::UpdateSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Session Interface Not Valid!"));
		//Broadcast Event 
		OnSessionUpdatedEvent.Broadcast(false);
		return;
	}
	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*SessionSettings));
	SessionSettings->Set(SETTING_MAPNAME, FString("MapName"), EOnlineDataAdvertisementType::ViaOnlineService);
	
	//Create Handle
	UpdateSessionCompleteDelegateHandle = sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);


	if (sessionInterface->UpdateSession(NAME_GameSession, *updatedSessionSettings))
	{
		SessionSettings = updatedSessionSettings;
	}
	else
	{
		//Failed to update session
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
		OnSessionUpdatedEvent.Broadcast(false);
	}

}

void UUT_SessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}
	OnSessionUpdatedEvent.Broadcast(Successful);
}
