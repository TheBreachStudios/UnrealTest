#include "UnrealTest/UnrealTestGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Math/UnrealMathUtility.h"

UUnrealTestGameInstance::UUnrealTestGameInstance(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UUnrealTestGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UUnrealTestGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UUnrealTestGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UUnrealTestGameInstance::OnJoinSessionComplete);
}

void UUnrealTestGameInstance::StartOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, 4);
}

void UUnrealTestGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), true, true);
}

void UUnrealTestGameInstance::JoinOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult SearchResult;

	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			// Filter sessions from ourself
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
			{
				SearchResult = SessionSearch->SearchResults[i];

				JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SearchResult);
				break;
			}
		}
	}
}

void UUnrealTestGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}

bool UUnrealTestGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, SessionMapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found"));
	}

	return false;
}

void UUnrealTestGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			bool bSucceded = Sessions->FindSessions(*UserId, SessionSearch.ToSharedRef());

			if(!bSucceded)
			{
				OnFindSessionsComplete(false);
			}
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

bool UUnrealTestGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UUnrealTestGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UUnrealTestGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), SessionMapName, true, "listen");
	}
}

void UUnrealTestGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// Try to join a session
			if (SessionSearch->SearchResults.Num() > 0)
			{
				ULocalPlayer* const Player = GetFirstGamePlayer();
				for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
				{
					// Filter sessions from ourself
					if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
					{
						FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[i];

						JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GenerateRandomSessionName(), SearchResult);
						break;
					}
				}
			}
		}
	}
}

void UUnrealTestGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UUnrealTestGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true);
			}
		}
	}
}

FName UUnrealTestGameInstance::GenerateRandomSessionName()
{
	FString out = "";
	for(int i = 0; i < RandomSessionNameLength; ++i)
	{
		int asciiValue = FMath::RandRange(0, 255);
		out.Append(FString::FromInt(asciiValue));
	}

	return FName(out);
}
