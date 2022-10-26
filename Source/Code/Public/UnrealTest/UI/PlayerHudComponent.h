// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHudComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTEST_API UPlayerHudComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerHudComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCharacterHudWidget> PlayerHudClass = nullptr;

	class UCharacterHudWidget* PlayerHUD = nullptr;	
};
