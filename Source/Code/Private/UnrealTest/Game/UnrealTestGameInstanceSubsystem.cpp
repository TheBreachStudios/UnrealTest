// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTest/Game/UnrealTestGameInstanceSubsystem.h"

// Unreal engine
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemUtils.h"

// Game Project
#include "Code/Public/UnrealTest/Game/UnrealTestGameMode.h"
#include "Code/Public/UnrealTest/Controller/UnrealTestPlayerController.h"

#pragma region Initialization
// Constructor.
UUnrealTestGameInstanceSubsystem::UUnrealTestGameInstanceSubsystem()
	: OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompletedEvent)),
	OnStartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompletedEvent)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompletedEvent)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompletedEvent)),
	MatchPhase(EMatchPhase::NONE)
{
	TeamList.ListOfTeams.Empty();
}
#pragma endregion Initialization

#pragma region Getters / Setters
// Get player Team ID.
int32 UUnrealTestGameInstanceSubsystem::GetPlayerTeamID(APlayerController* PlayerController)
{
	bool playerFound = false;
	for (int i = 0; i < TeamList.ListOfTeams.Num(); i++)
	{
		for (APlayerController* tempPlayer : TeamList.ListOfTeams[i].TeamMembers)
		{
			if (tempPlayer == PlayerController) { return i; }
		}
	}

	return -1;
}
#pragma endregion Getters / Setters

#pragma region Functions

// Translate BP join sessions complete result to CPP join sessions sessions complete result.
EOnJoinSessionCompleteResult::Type UUnrealTestGameInstanceSubsystem::TranslateToCPPOnJoinSessionCompleteResult(EBPOnJoinSessionCompleteResult JoinResult)
{
	return (EOnJoinSessionCompleteResult::Type)JoinResult;
}

// Translate CPP join sessions complete result to BP join sessions sessions complete result.
EBPOnJoinSessionCompleteResult UUnrealTestGameInstanceSubsystem::TranslateToBPOnJoinSessionCompleteResult(EOnJoinSessionCompleteResult::Type JoinResult)
{
	return (EBPOnJoinSessionCompleteResult)JoinResult;
}
// On create session completed event.
void UUnrealTestGameInstanceSubsystem::OnCreateSessionCompletedEvent(FName SessionName, bool Successful)
{
	// Clears previous OnCreateSessionComplete delegate
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	AUnrealTestGameMode* gameMode = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) {
		UE_LOG(LogTemp, Fatal, TEXT("GameMode is not AUnrealTestGameMode."));
	}

	// Broadast session creation has been completed
	OnCreateSessionCompleted.Broadcast(Successful);
}

// On session start completed event.
void UUnrealTestGameInstanceSubsystem::OnStartSessionCompletedEvent(FName SessionName, bool Successful)
{
	// Clears previous OnStartSessionCompleted delegate
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	// Broadast session start has been completed
	OnStartSessionCompleted.Broadcast(Successful);
}

// On session find completed event.
void UUnrealTestGameInstanceSubsystem::OnFindSessionsCompletedEvent(bool Successful)
{
	// Clears previous OnFindSessionsCompleted delegate
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	// Broadast session search has been completed and send the results (no results in this case).
	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleted.Broadcast(TArray<FBlueprintSessionResult>(), Successful);
		return;
	}

	// Broadast session search has been completed and send the results after translating to a BP readable struct.
	TArray<FBlueprintSessionResult> tempFBlueprintSessionResults;
	for (FOnlineSessionSearchResult sessionResult : LastSessionSearch->SearchResults)
	{
		FBlueprintSessionResult tempSessionResult;
		tempSessionResult.OnlineResult = sessionResult;
		tempFBlueprintSessionResults.Add(tempSessionResult);
	}
	OnFindSessionsCompleted.Broadcast(tempFBlueprintSessionResults, Successful);
}

// On session join completed event.
void UUnrealTestGameInstanceSubsystem::OnJoinSessionCompletedEvent(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// Clears previous OnJoinSessionCompleted delegate
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	// Broadast session join has been completed
	AUnrealTestGameMode* gameMode = Cast<AUnrealTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) {
		UE_LOG(LogTemp, Fatal, TEXT("GameMode is not AUnrealTestGameMode."));
	}

	OnJoinGameSessionCompleted.Broadcast(TranslateToBPOnJoinSessionCompleteResult(Result));
}

// Start game session.
void UUnrealTestGameInstanceSubsystem::StartSession()
{
	// If no session interface is found broadcast session didn't start
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnStartSessionCompleted.Broadcast(false);
		return;
	}

	// Bind session started event to OnStartSessionCompleteDelegate
	StartSessionCompleteDelegateHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

	// Try start session, if it fails broadcast session didn't start and clear previous delegate
	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleted.Broadcast(false);
	}
}

// Create game session.
void UUnrealTestGameInstanceSubsystem::CreateSession(int32 MaxPublicConections, bool IsLANMatch)
{
	// If no session interface is found broadcast session couldn't be created
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnCreateSessionCompleted.Broadcast(false);
		return;
	}

	// Prepare session settings
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

	LastSessionSettings->Set(SETTING_MAPNAME, FString("WaitingRoom"), EOnlineDataAdvertisementType::ViaOnlineService);
	
	// Bind session creation event to OnCreateSessionCompleteDelegate
	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	// Try create session, if it fails broadcast session didn't start and clear previous delegate
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleted.Broadcast(false);
	}
}

// Find game session.
void UUnrealTestGameInstanceSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	// If no session interface is found broadcast sessions couldn't be found
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnFindSessionsCompleted.Broadcast(TArray<FBlueprintSessionResult>(), false);
		return;
	}

	// Bind sessions found event to OnFindSessionsCompleteDelegate
	FindSessionsCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	// Try find sessions, if it fails broadcast session didn't start and clear previous delegate
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleted.Broadcast(TArray<FBlueprintSessionResult>(), false);
	}
}

// Join game session.
void UUnrealTestGameInstanceSubsystem::JoinGameSession(const FBlueprintSessionResult& SessionResult)
{

	// If no session interface is found broadcast sessions couldn't be joined to
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnJoinGameSessionCompleted.Broadcast(EBPOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// Bind session joined event to OnJoinSessionCompleteDelegate
	JoinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	// Try join sessiom, if it fails broadcast session didn't start and clear previous delegate
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult.OnlineResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleted.Broadcast(EBPOnJoinSessionCompleteResult::UnknownError);
	}
}

// Try travel to current session.
bool UUnrealTestGameInstanceSubsystem::TryTravelToCurrentSession()
{
	// If no session interface is found broadcast sessions return failure
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return false;
	}

	// If no connectString is found broadcast sessions return failure
	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return false;
	}


	// Travel to listen server from client and send selected champion name to server through a URL param
	AUnrealTestPlayerController* playerController = Cast<AUnrealTestPlayerController>(GetWorld()->GetFirstPlayerController());
	playerController->ClientTravel(connectString + "?champ=" + playerController->GetPlayerPawnClassName().ToString(), TRAVEL_Absolute);
	return true;
}

// Add player to team list.
void UUnrealTestGameInstanceSubsystem::AddPlayerToTeam(int32 TeamID, APlayerController* NewPlayer)
{
	// If TeamID exist as key, add new player to TeamList
	if (TeamList.ListOfTeams.IsValidIndex(TeamID))
	{
		TeamList.ListOfTeams[TeamID].TeamMembers.AddUnique(NewPlayer);
		AlivePlayersPerTeam[TeamID] = AlivePlayersPerTeam[TeamID] + 1;
	}
	// if NOT create a new Team and add the player to it
	else
	{
		FTeam tempTeam;
		tempTeam.TeamMembers.AddUnique(NewPlayer);
		TeamList.ListOfTeams.Add(tempTeam);
		AlivePlayersPerTeam.Add(1);
	}
}

// On player died update info.
void UUnrealTestGameInstanceSubsystem::OnPlayerDied(int32 TeamID)
{
	// Decrease alive players from TeamID
	if (AlivePlayersPerTeam.IsValidIndex(TeamID))
	{
		AlivePlayersPerTeam[TeamID] = AlivePlayersPerTeam[TeamID] - 1;

		// If was the last player alive, broadcast that the laste team member has died
		if (AlivePlayersPerTeam[TeamID] == 0) {
			OnLastTeamPlayerDied.Broadcast(TeamID);
		}
	}
}
#pragma endregion Functions