// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UnrealTestGameMode.generated.h"



UCLASS(minimalapi)
class AUnrealTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealTestGameMode();

    /* Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UnrealTestNet")
        FORCEINLINE FString GetMatchName() const { return MatchGame; }

    UFUNCTION(BlueprintCallable, Category = "UnrealTestNet")
        void SetMatchName(FString name);

    /*TO-DO: Heroes enum and match state enum*/

    /*TO-DO: Team matchmaking based in player's level*/



protected:
    /* Called when the game starts. */
    virtual void BeginPlay() override;

    /* The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    /* The widget instance that we are using as our menu. */
    UPROPERTY()
        UUserWidget* CurrentWidget;

    /* Registered matchName */
    UPROPERTY()
        FString MatchGame = "";


};



