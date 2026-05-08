// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/HealthComponent.h"
#include "Character/AttackComponent.h"
#include "Character/MyCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "FirstCharacter.generated.h"

UCLASS()
class FIGHTINGGAME_API AFirstCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere)
	UAttackComponent* AttackComponent;

	UPROPERTY(EditAnywhere)
	UMyCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Shift;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Tab;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_LightAttack;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_DI;


	void Move(const FInputActionValue& Value);
	void StartJump();
	void PressDown();
	void EndPressDown();
	void PressShift();
	void EndPressShift();
	void PressTab();
	void EndPressTab();
	void LightAttack();
	void CheckDirection(const FInputActionValue& Value);
	void ResetDirection();
};
