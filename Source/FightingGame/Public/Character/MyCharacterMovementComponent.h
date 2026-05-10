// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EFighterMovementState : uint8
{
	Idle,
	Walking,
	Jumping,
	FastFalling,
	Sprinting,
	AirDash,
	DoubleJump
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	None,
	Attacking,
	Shield
};

UCLASS()
class FIGHTINGGAME_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UMyCharacterMovementComponent();

	void MoveForward(float Direction);
	void StartJump();
	void PressDown();
	void EndPressDown();
	void PressShift();
	void EndPressShift();
	void PressTab();
	void EndPressTab();
	void LightAttack();
	void CheckDirection(FVector2D Input);


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float CalculateSpeed() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EFighterMovementState FighterMovementState = EFighterMovementState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EActionState ActionState = EActionState::None;

protected:

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float BaseSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float MoveSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float FastFallStrength = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float SprintStrength = 1.7f;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float AirHorizontalControl = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float AirDashStrength = 600.0f;

	UPROPERTY(EditAnywhere, Category = "DI")
	float DI_X = 0.0f;

	UPROPERTY(EditAnywhere, Category = "DI")
	float DI_Z = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Jump Feeling")
	float UpGravity = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Jump Feeling")
	float DownGravity = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Jump Feeling")
	float ShortHopGravity = 5.0f;


	UPROPERTY(EditAnywhere, Category = "Jump Feeling")
	float ShortHopSpeedMulti = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Air Dash")
	float AirDashDuration = 0.10f;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	float LightAttackDuration = 0.40f;

private:
	bool bSprintHeld = false;
	bool bDownHeld = false;
	bool bTabHeld = false;
	bool bShortHop = false;
	int MaxJumps = 2;
	int JumpsRemaining = 0;
	int AirDashesRemaining = 0;
	int MaxAirDash = 1;

	float AirDashTimer = 0.0f;
	float AttackTimer = 0.0f;

	bool bIsMovingHorizontally = false;

};
