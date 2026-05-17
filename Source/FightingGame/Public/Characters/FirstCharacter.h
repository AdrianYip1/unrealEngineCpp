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
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "FirstCharacter.generated.h"

UCLASS()
class FIGHTINGGAME_API AFirstCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnHitboxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, 
		bool bFromSweep,
		const FHitResult& SweepResult
	);

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

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* HeadHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* TorsoHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* RightArmHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* LeftArmHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* RightForearmHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* LeftForearmHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* LeftThighHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* LeftCalfHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* RightThighHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hurtbox")
	UBoxComponent* RightCalfHurtbox;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
	UBoxComponent* RightHandHitbox;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
	UBoxComponent* LeftHandHitbox;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
	UBoxComponent* RightFootHitbox;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
	UBoxComponent* LeftFootHitbox;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget;


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
	void ReceiveDamage(float Damage);
	void RecieveKnockback(float Damage, EAttackType AttackType, float KnockbackDir);
};	
