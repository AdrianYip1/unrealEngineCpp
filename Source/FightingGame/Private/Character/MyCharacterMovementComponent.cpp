// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MyCharacterMovementComponent.h"
#include "GameFramework/Character.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	bConstrainToPlane = true;
	PlaneConstraintNormal = FVector(0.0f, 1.0f, 0.0f);
	AirControl = 0.45;
}

void UMyCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FighterMovementState == EFighterMovementState::AirDash && !IsMovingOnGround()) {
		AirDashTimer -= DeltaTime;
		if (AirDashTimer <= 0.0f) {
			FighterMovementState = EFighterMovementState::Jumping;
			GravityScale = UpGravity;
		}
	}

	if (ActionState == EActionState::Attacking) {
		AttackTimer -= DeltaTime;
		if (AttackTimer <= 0.0f) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack End (Light)"));
			ActionState = EActionState::None;
		}
	}

	if (FighterMovementState == EFighterMovementState::Jumping && IsMovingOnGround()) {
		FighterMovementState = (bSprintHeld) ? EFighterMovementState::Sprinting : EFighterMovementState::Idle;
		JumpsRemaining = 0;
		GravityScale = UpGravity;
		bShortHop = false;
		if (bTabHeld) ActionState = EActionState::Shield;
	}

	if (FighterMovementState == EFighterMovementState::FastFalling && IsMovingOnGround()) {
		FighterMovementState = EFighterMovementState::Idle;
		if (bTabHeld) ActionState = EActionState::Shield;
		bShortHop = false;
	}

	if (FighterMovementState == EFighterMovementState::AirDash && IsMovingOnGround()) {
		FighterMovementState = (bSprintHeld) ? EFighterMovementState::Sprinting : EFighterMovementState::Idle;
		if (bTabHeld) ActionState = EActionState::Shield;
		bShortHop = false;
	}

	switch (FighterMovementState) {
	case EFighterMovementState::FastFalling:
		Velocity += FVector(0.0f, 0.0f, -FastFallStrength);
		break;
	default:
		break;
	}

	if (IsFalling() && FighterMovementState != EFighterMovementState::AirDash) {
		float FallGrav = (bShortHop && Velocity.Z < 0.0f) ? ShortHopGravity : DownGravity;
		GravityScale = (Velocity.Z < 0.0f) ? FallGrav : UpGravity;
	}
}

float UMyCharacterMovementComponent::CalculateSpeed() const
{
	float Speed = BaseSpeed;

	switch (FighterMovementState) {
	case EFighterMovementState::Sprinting:
		Speed *= SprintStrength;
		break;
	default:
		break;
	}

	return Speed;
}

void UMyCharacterMovementComponent::MoveForward(float Direction)
{
	if (GetCharacterOwner() && (ActionState == EActionState::None) && (FighterMovementState != EFighterMovementState::AirDash))
	{
		if (IsFalling())
		{
			// Air movement
			float AirSpeed = BaseSpeed * (bShortHop ? ShortHopSpeedMulti : 0.7f);
			AddInputVector(FVector(Direction * AirSpeed, 0.0f, 0.0f));

		}
		else
		{
			// Ground movement — set MaxWalkSpeed based on state
			MaxWalkSpeed = CalculateSpeed();
			GetCharacterOwner()->AddMovementInput(FVector(1, 0, 0), Direction);
		}
	}
}

void UMyCharacterMovementComponent::StartJump()
{

	if (IsMovingOnGround() && ActionState == EActionState::None) {
		if (bDownHeld) {
			//short hop

			bShortHop = true;
			Velocity.Z = 250.0f;
			ActionState = EActionState::None;
			FighterMovementState = EFighterMovementState::Jumping;

			SetMovementMode(MOVE_Falling);
			JumpsRemaining = MaxJumps - 1;
			AirDashesRemaining = MaxAirDash;
		}
		else {
			ActionState = EActionState::None;
			FighterMovementState = EFighterMovementState::Jumping;
			Velocity += FVector(0.0f, 0.0f, 450.0f);
			SetMovementMode(MOVE_Falling);
			JumpsRemaining = MaxJumps - 1;
			AirDashesRemaining = MaxAirDash;
		}
	}
	else if (IsFalling() && JumpsRemaining > 0 && ActionState == EActionState::None) {
		FighterMovementState = EFighterMovementState::Jumping;
		Velocity.Z = 450.0f;
		JumpsRemaining--;
	}
	
}

void UMyCharacterMovementComponent::PressDown()
{
	bDownHeld = true;
	if (GetCharacterOwner() && IsFalling()) {
		FighterMovementState = EFighterMovementState::FastFalling;
	}
}

void UMyCharacterMovementComponent::EndPressDown()
{
	bDownHeld = false;

}

void UMyCharacterMovementComponent::PressShift()
{
	bSprintHeld = true;
	if (GetCharacterOwner() && IsMovingOnGround()) {
		FighterMovementState = EFighterMovementState::Sprinting;
	}
	else if (GetCharacterOwner() && IsFalling() && AirDashesRemaining > 0 && ActionState == EActionState::None) {

		FVector DashDir = FVector(FMath::Sign(DI_X), 0.0f, FMath::Sign(DI_Z)).GetSafeNormal();

		Velocity = DashDir * AirDashStrength;

		if (DashDir.IsNearlyZero()) {
			// TODO: spot dodge
			AirDashesRemaining--;
			FighterMovementState = EFighterMovementState::AirDash;
			AirDashTimer = AirDashDuration;
			return;
		}
		if (DI_Z >= 0.0f) GravityScale = 0.0f;

		FighterMovementState = EFighterMovementState::AirDash;
		AirDashesRemaining--;
		AirDashTimer = AirDashDuration;

	}
}

void UMyCharacterMovementComponent::EndPressShift()
{
	bSprintHeld = false;
	if (FighterMovementState == EFighterMovementState::Sprinting) {
		FighterMovementState = EFighterMovementState::Idle;
	}
}

void UMyCharacterMovementComponent::PressTab()
{
	bTabHeld = true;
	if (IsMovingOnGround()) {
		ActionState = EActionState::Shield;
	}
}

void UMyCharacterMovementComponent::EndPressTab()
{
	bTabHeld = false;
	ActionState = EActionState::None;

}

void UMyCharacterMovementComponent::LightAttack()
{
	if (ActionState == EActionState::None) {
		ActionState = EActionState::Attacking;
		GEngine->AddOnScreenDebugMessage(-1, 5.f,FColor::Red, TEXT("Attack Start (Light)"));
		AttackTimer = LightAttackDuration;
	}


}

void UMyCharacterMovementComponent::CheckDirection(FVector2D Input) {
	DI_X = Input.X;
	DI_Z = Input.Y;
}