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

	HeadHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Head Hurtbox"));
	HeadHurtbox->SetupAttachment(GetMesh(), FName("head_socket"));

	TorsoHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Torso Hurtbox"));
	TorsoHurtbox->SetupAttachment(GetMesh(), FName("torso_socket"));

	RightArmHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Arm Hurtbox"));
	RightArmHurtbox->SetupAttachment(GetMesh(), FName("right_arm_socket"));

	LeftArmHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Arm Hurtbox"));
	LeftArmHurtbox->SetupAttachment(GetMesh(), FName("left_arm_socket"));

	RightForearmHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Forearm Hurtbox"));
	RightForearmHurtbox->SetupAttachment(GetMesh(), FName("right_forearm_socket"));

	LeftForearmHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Forearm Hurtbox"));
	LeftForearmHurtbox->SetupAttachment(GetMesh(), FName("left_forearm_socket"));

	LeftThighHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Thigh Hurtbox"));
	LeftThighHurtbox->SetupAttachment(GetMesh(), FName("left_thigh_socket"));

	LeftCalfHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Calf Hurtbox"));
	LeftCalfHurtbox->SetupAttachment(GetMesh(), FName("left_calf_socket"));

	RightThighHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Thigh Hurtbox"));
	RightThighHurtbox->SetupAttachment(GetMesh(), FName("right_thigh_socket"));

	RightCalfHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Calf Hurtbox"));
	RightCalfHurtbox->SetupAttachment(GetMesh(), FName("right_calf_socket"));

	RightHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Hitbox"));
	RightHandHitbox->SetupAttachment(GetMesh(), FName("right_hand_socket"));

	LeftHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Hitbox"));
	LeftHandHitbox->SetupAttachment(GetMesh(), FName("left_hand_socket"));

	RightFootHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Foot Hitbox"));
	RightFootHitbox->SetupAttachment(GetMesh(), FName("right_foot_socket"));

	LeftFootHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Foot Hitbox"));
	LeftFootHitbox->SetupAttachment(GetMesh(), FName("left_foot_socket"));
}

void AFirstCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	RightHandHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFirstCharacter::OnHitboxBeginOverlap);
	LeftHandHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFirstCharacter::OnHitboxBeginOverlap);

	RightFootHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFirstCharacter::OnHitboxBeginOverlap);
	LeftFootHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFirstCharacter::OnHitboxBeginOverlap);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->GetLocalPlayer()->GetControllerId() == 0 && HUDWidgetClass) {
		HUDWidget = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
		HUDWidget->AddToViewport();
	}
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

void AFirstCharacter::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	if (!MovementComponent->isAttacking()) return;
	if (!OtherComp->GetName().Contains(TEXT("Hurtbox"))) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("Used Attack: %s"), *UEnum::GetValueAsString(MovementComponent->GetCurrentAttackType())));

	if (AFirstCharacter* Target = Cast<AFirstCharacter>(OtherActor)) {
		float Damage = 0.0f;
		if (MovementComponent) {
			switch (MovementComponent->GetCurrentAttackType())
			{
				case EAttackType::GroundNeutral: Damage = 10.0f; break;
				case EAttackType::GroundForward: Damage = 10.0f; break;
				case EAttackType::GroundDown: Damage = 10.0f; break;
				case EAttackType::GroundUp: Damage = 10.0f; break;

				case EAttackType::AerialNeutral: Damage = 10.0f; break;
				case EAttackType::AerialSide: Damage = 10.0f; break;
				case EAttackType::AerialDown: Damage = 10.0f; break;
				case EAttackType::AerialUp: Damage = 10.0f; break;
				default: break;
			}
		}

		// KnockbackDir: +1 if target is to the right of attacker, -1 if to the left
		float KnockbackDir = (Target->GetActorLocation().X >= GetActorLocation().X) ? 1.0f : -1.0f;
		Target->ReceiveDamage(Damage);
		Target->RecieveKnockback(Damage, MovementComponent->GetCurrentAttackType(), KnockbackDir);
	}
}

void AFirstCharacter::ReceiveDamage(float Damage)
{
	if (HealthComponent) HealthComponent->TakeDamage(Damage);
}

void AFirstCharacter::RecieveKnockback(float Damage, EAttackType AttackType, float KnockbackDir) {
	if (MovementComponent && HealthComponent)
		MovementComponent->DealKnockback(HealthComponent->GetHealthPercentage(), Damage, AttackType, KnockbackDir);
}