#include "Character/MyCharacterMovementComponent.h"
#include "GameFramework/Character.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	bConstrainToPlane = true;
	PlaneConstraintNormal = FVector(0.0f, 1.0f, 0.0f);
	AirControl = 0.45f;
	bRunPhysicsWithNoController = true;
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
			ActionState = EActionState::None;
			CurrentAttackType = EAttackType::None;
		}
	}

	if (ActionState == EActionState::Knockback) {
		KnockbackTimer -= DeltaTime;
		if (KnockbackTimer <= 0.0f)
			ActionState = EActionState::None;
	}

	if ((FighterMovementState == EFighterMovementState::Jumping ||
		FighterMovementState == EFighterMovementState::DoubleJump) && IsMovingOnGround()) {
		if (bSprintHeld)
			FighterMovementState = EFighterMovementState::Sprinting;
		else if (bIsMovingHorizontally)
			FighterMovementState = EFighterMovementState::Walking;
		else
			FighterMovementState = EFighterMovementState::Idle;
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
		GravityScale = UpGravity;
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

	if (IsMovingOnGround()
		&& FighterMovementState != EFighterMovementState::Jumping
		&& FighterMovementState != EFighterMovementState::DoubleJump
		&& FighterMovementState != EFighterMovementState::AirDash
		&& ActionState == EActionState::None)
	{
		if (bSprintHeld && bIsMovingHorizontally)
			FighterMovementState = EFighterMovementState::Sprinting;
		else if (!bSprintHeld)
			FighterMovementState = bIsMovingHorizontally ? EFighterMovementState::Walking : EFighterMovementState::Idle;
	}

	bIsMovingHorizontally = false;
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

		bIsMovingHorizontally = (Direction != 0.0f);
		if (Direction > 0.0f) bFacingRight = true;
		else if (Direction < 0.0f) bFacingRight = false;
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
		FighterMovementState = EFighterMovementState::DoubleJump;
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
	if (GetCharacterOwner() && IsFalling() && AirDashesRemaining > 0 && ActionState == EActionState::None) {

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
		FighterMovementState = bIsMovingHorizontally ? EFighterMovementState::Walking : EFighterMovementState::Idle;
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
	if (ActionState == EActionState::Shield)
		ActionState = EActionState::None;
}

void UMyCharacterMovementComponent::LightAttack()
{
	if (ActionState != EActionState::None) return;

	ActionState = EActionState::Attacking;
	AttackTimer = LightAttackDuration;

	const bool bAerial = IsFalling();
	const float DirX = DI_X;
	const float DirZ = DI_Z;

	if (bAerial)
	{
		if (DirZ < 0.0f)
			CurrentAttackType = EAttackType::AerialDown;
		else if (DirZ > 0.0f)
			CurrentAttackType = EAttackType::AerialUp;
		else if (DirX != 0.0f)
			CurrentAttackType = EAttackType::AerialSide;
		else
			CurrentAttackType = EAttackType::AerialNeutral;
	}
	else
	{
		if (bDownHeld || DirZ < 0.0f)
			CurrentAttackType = EAttackType::GroundDown;
		else if (DirZ > 0.0f)
			CurrentAttackType = EAttackType::GroundUp;
		else if (DirX != 0.0f)
			CurrentAttackType = EAttackType::GroundForward;
		else
			CurrentAttackType = EAttackType::GroundNeutral;
	}
}

void UMyCharacterMovementComponent::CheckDirection(FVector2D Input) {
	DI_X = Input.X;
	DI_Z = Input.Y;
}

FVector2D UMyCharacterMovementComponent::getDI() const {
	return FVector2D(DI_X, DI_Z);
}

bool UMyCharacterMovementComponent::inAir() {
	return (!IsMovingOnGround());
}

bool UMyCharacterMovementComponent::canAttack() {
	return (ActionState == EActionState::None);
}

bool UMyCharacterMovementComponent::isAttacking() {
	return (ActionState == EActionState::Attacking);
}

void UMyCharacterMovementComponent::DealKnockback(float RemainingHealth, float Damage, EAttackType AttackType, float KnockbackDir)
{
	float force = 300.0f + (1.0f - RemainingHealth) * Damage * 50.0f;

	FVector Launch = FVector::ZeroVector;
	switch (AttackType)
	{
	case EAttackType::GroundNeutral: Launch = FVector(KnockbackDir * force * 0.6f, 0.0f, force * 0.8f); break;
	case EAttackType::GroundForward: Launch = FVector(KnockbackDir * force, 0.0f, force * 0.4f); break;
	case EAttackType::GroundDown:    Launch = FVector(KnockbackDir * force * 0.3f, 0.0f, -force); break;
	case EAttackType::GroundUp:      Launch = FVector(KnockbackDir * force * 0.2f, 0.0f, force * 1.5f); break;

	case EAttackType::AerialNeutral: Launch = FVector(KnockbackDir * force * 0.6f, 0.0f, force * 0.8f); break;
	case EAttackType::AerialSide:    Launch = FVector(KnockbackDir * force, 0.0f, force * 0.3f); break;
	case EAttackType::AerialDown:    Launch = FVector(KnockbackDir * force * 0.3f, 0.0f, -force * 1.2f); break;
	case EAttackType::AerialUp:      Launch = FVector(KnockbackDir * force * 0.2f, 0.0f, force * 1.5f); break;
	default: break;
	}

	ActionState = EActionState::Knockback;
	KnockbackTimer = 0.4f;

	Velocity = Launch;
	SetMovementMode(MOVE_Falling);
}