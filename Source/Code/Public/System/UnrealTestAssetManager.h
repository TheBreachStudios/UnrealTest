// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UnrealTestAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API UUnrealTestAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	UUnrealTestAssetManager();
	
	// Returns the AssetManager singleton object.
	static UUnrealTestAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;

private:

	void InitializeTagsSystem();
};
