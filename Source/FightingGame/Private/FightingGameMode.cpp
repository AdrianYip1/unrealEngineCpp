// Fill out your copyright notice in the Description page of Project Settings.

#include "FightingGameMode.h"
#include "Characters/FirstCharacter.h"
#include "GameCamera.h"
#include "Kismet/GameplayStatics.h"

void AFightingGameMode::BeginPlay() {
	Super::BeginPlay();

	GameCamera = Cast<AGameCamera>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameCamera::StaticClass())
	);

	PC = GetWorld()->GetFirstPlayerController();
	if (PC) P1 = Cast<AFirstCharacter>(PC->GetPawn());
	if (PC && GameCamera) PC->SetViewTargetWithBlend(GameCamera);
}
