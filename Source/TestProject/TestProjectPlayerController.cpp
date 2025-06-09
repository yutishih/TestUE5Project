// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "TestProjectCameraManager.h"

ATestProjectPlayerController::ATestProjectPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ATestProjectCameraManager::StaticClass();
}

void ATestProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
