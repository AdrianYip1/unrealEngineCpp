// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCamera.h"

AGameCamera::AGameCamera()
{
	// Face toward the origin (positive Y direction)
	SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}
