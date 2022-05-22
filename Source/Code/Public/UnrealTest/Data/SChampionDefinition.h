#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

// Game Project
#include "Code/Public/UnrealTest/Character/UnrealTestCharacter.h"
#include "SChampionDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FChampionDefinition : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    // Champion name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Champion")
    FText ChampionName;

    // Champion class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Champion")
    TSubclassOf<class AUnrealTestCharacter> ChampionClass;

    // Champion image
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Champion")
    UTexture2D* ChampionTexture;
};
