// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"

// Unreal engine
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemUtils.h"

// Game Project
#include "Code/Public/UnrealTest/Game/UnrealTestGameMode.h"

#pragma region Initialization
// Constructor
UUnrealTestGameInstanceSubsystem::UUnrealTestGameInstanceSubsystem()
	: OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompletedEvent)),
	OnStartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompletedEvent)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompletedEvent)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompletedEvent)),
	GamePhase(EMatchPhase::NONE)
{
}
#pragma endregion Initialization

#pragma region Functions

EOnJoinSessionCompleteResult::Type UUnrealTestGameInstanceSubsystem::TranslateToCPPOnJoinSessionCompleteResult(EBPOnJoinSessionCompleteResult JoinResult)
{
	return (EOnJoinSessionCompleteResult::Type)JoinResult;
}

EBPOnJoinSessionCompleteResult UUnrealTestGameInstanceSubsystem::TranslateToBPOnJoinSessionCompleteResult(EOnJoinSessionCompleteResult::Type JoinResult)
{
	return (EBPOnJoinSessionCompleteResult)JoinResult;
}

// Create game session
void UUnrealTestGameInstanceSubsystem::CreateSession(int32 MaxPublicConections, bool IsLANMatch)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnCreateSessionCompleted.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = MaxPublicConections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(SETTING_MAPNAME, FString("TestMap"), EOnlineDataAdvertisementType::ViaOnlineService);

	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleted.Broadcast(false);
	}
}

// On create session completed event
void UUnrealTestGameInstanceSubsystem::OnCreateSessionCompletedEvent(FName SessionName, bool Successful)
{

	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	AUnrealTestGameMode* gameMode = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) {
		UE_LOG(LogTemp, Fatal, TEXT("GameMode is not AUnrealTestGameMode."));
	}

	OnCreateSessionCompleted.Broadcast(Successful);
}

// Start game session
void UUnrealTestGameInstanceSubsystem::StartSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnStartSessionCompleted.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleted.Broadcast(false);
	}
}

// On session start completed event
void UUnrealTestGameInstanceSubsystem::OnStartSessionCompletedEvent(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnStartSessionCompleted.Broadcast(Successful);
}

void UUnrealTestGameInstanceSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnFindSessionsCompleted.Broadcast(TArray<FBlueprintSessionResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleted.Broadcast(TArray<FBlueprintSessionResult>(), false);
	}
}

void UUnrealTestGameInstanceSubsystem::OnFindSessionsCompletedEvent(bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleted.Broadcast(TArray<FBlueprintSessionResult>(), Successful);
		return;
	}

	TArray<FBlueprintSessionResult> tempFBlueprintSessionResults;
	for (FOnlineSessionSearchResult sessionResult : LastSessionSearch->SearchResults)
	{
		FBlueprintSessionResult tempSessionResult;
		tempSessionResult.OnlineResult = sessionResult;
		tempFBlueprintSessionResults.Add(tempSessionResult);
	}
	OnFindSessionsCompleted.Broadcast(tempFBlueprintSessionResults, Successful);
}

void UUnrealTestGameInstanceSubsystem::JoinGameSession(const FBlueprintSessionResult& SessionResult)
{

	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnJoinGameSessionCompleted.Broadcast(EBPOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult.OnlineResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleted.Broadcast(EBPOnJoinSessionCompleteResult::UnknownError);
	}
}

void UUnrealTestGameInstanceSubsystem::OnJoinSessionCompletedEvent(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	AUnrealTestGameMode* gameMode = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) {
		UE_LOG(LogTemp, Fatal, TEXT("GameMode is not AUnrealTestGameMode."));
	}

	OnJoinGameSessionCompleted.Broadcast(TranslateToBPOnJoinSessionCompleteResult(Result));

	UE_LOG(LogTemp, Warning, TEXT("[UUnrealTestGameInstanceSubsystem] NumSessions: %i"), sessionInterface->GetNumSessions());
}

bool UUnrealTestGameInstanceSubsystem::TryTravelToCurrentSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return false;
	}

	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}
#pragma endregion Functions
