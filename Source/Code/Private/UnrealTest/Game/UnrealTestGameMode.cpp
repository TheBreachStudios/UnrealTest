// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Game/UnrealTestGameMode.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AUnrealTestGameMode::AUnrealTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    PrimaryActorTick.bCanEverTick = true;
}

void AUnrealTestGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

/*
In game mode the management of state changes.For the management of states we will have different UserWidgets in which we will manage GUI eventsand call
our own functions that we will declare in the GameMode.The first thing will be to declare a function to change widgets, 
as well as a current widgetand an initial one.This function will be responsible for calling the AddToViewport() functions to the incoming widgetand The Outgoing RemoveFromViewport() functions.
*/

void AUnrealTestGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AUnrealTestGameMode::OnServerButtonClick(FString sPort)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *FString("Server"));
    //m_pManager->activateAsServer(FCString::Atoi(*sPort));
    //TO-DO: SERVER ACTIVATION PASSING PORT 
}

void AUnrealTestGameMode::OnClientButtonClick(FString sIP, FString sPort)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *FString("Client"));
    //TO-DO: CLIENT CONNECTION PASSING IP AND PORT
}

void AUnrealTestGameMode::OnServerStartButtonClick()
{
    //TO-DO: Map where the lobby is and then map with four player starts
    const char* sName = "ThirdPersonMap";
    UGameplayStatics::OpenLevel(GetWorld(), sName);
    //GameBuffer data;
    //NetMessageType type = NetMessageType::LOAD_MAP;
    //data.write(type);
    //data.write(sName);
    //m_pManager->send(data.getbuffer(), data.getSize());
}
