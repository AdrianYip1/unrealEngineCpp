// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AttackComponent.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();	
}