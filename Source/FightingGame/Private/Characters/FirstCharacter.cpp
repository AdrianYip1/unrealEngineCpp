#include "Characters/FirstCharacter.h"
#include "Character/HealthComponent.h"
#include "Character/AttackComponent.h"
#include "Character/MyCharacterMovementComponent.h"

AFirstCharacter::AFirstCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>
		(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bFindCameraComponentWhenViewTarget = false;
	bUseControllerRotationYaw = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	MovementComponent = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());

	SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void AFirstCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFirstCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			Subsystem->AddMappingContext(IMC, 0);
	}
}

void AFirstCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AFirstCharacter::Move);
	EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AFirstCharacter::StartJump);
	EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AFirstCharacter::PressDown);
	EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &AFirstCharacter::EndPressDown);
	EIC->BindAction(IA_Shift, ETriggerEvent::Started, this, &AFirstCharacter::PressShift);
	EIC->BindAction(IA_Shift, ETriggerEvent::Completed, this, &AFirstCharacter::EndPressShift);
	EIC->BindAction(IA_Tab, ETriggerEvent::Started, this, &AFirstCharacter::PressTab);
	EIC->BindAction(IA_Tab, ETriggerEvent::Completed, this, &AFirstCharacter::EndPressTab);
	EIC->BindAction(IA_LightAttack, ETriggerEvent::Started, this, &AFirstCharacter::LightAttack);
	EIC->BindAction(IA_DI, ETriggerEvent::Triggered, this, &AFirstCharacter::CheckDirection);
	EIC->BindAction(IA_DI, ETriggerEvent::Completed, this, &AFirstCharacter::ResetDirection);
}

void AFirstCharacter::Move(const FInputActionValue& Value)
{
	float Dir = Value.Get<float>();
	if (MovementComponent) MovementComponent->MoveForward(Dir);
}

void AFirstCharacter::StartJump()
{
	if (MovementComponent) MovementComponent->StartJump();
}

void AFirstCharacter::PressDown()
{
	if (MovementComponent) MovementComponent->PressDown();
}

void AFirstCharacter::EndPressDown()
{
	if (MovementComponent) MovementComponent->EndPressDown();
}

void AFirstCharacter::PressShift()
{
	if (MovementComponent) MovementComponent->PressShift();
}

void AFirstCharacter::EndPressShift()
{
	if (MovementComponent) MovementComponent->EndPressShift();
}

void AFirstCharacter::PressTab()
{
	if (MovementComponent) MovementComponent->PressTab();
}

void AFirstCharacter::EndPressTab()
{
	if (MovementComponent) MovementComponent->EndPressTab();
}

void AFirstCharacter::LightAttack()
{
	if (MovementComponent) MovementComponent->LightAttack();
}

void AFirstCharacter::CheckDirection(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	if (MovementComponent) MovementComponent->CheckDirection(Input);
}

void AFirstCharacter::ResetDirection() {
	if (MovementComponent) MovementComponent->CheckDirection(FVector2D::ZeroVector);
}