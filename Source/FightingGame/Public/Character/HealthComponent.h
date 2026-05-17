// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIGHTINGGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	void TakeDamage(float damage);
	float GetHealthPercentage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly) float MaxHealth = 200.f;
	UPROPERTY(BlueprintReadOnly) float CurrentHealth;

protected:
	virtual void BeginPlay() override;
};
