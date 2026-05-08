// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Characters/FirstCharacter.h"
#include "GameCamera.h"
#include "FightingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTINGGAME_API AFightingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	AGameCamera* GameCamera;

	UPROPERTY()
	APlayerController* PC;

	UPROPERTY()
	AFirstCharacter* P1;

};

