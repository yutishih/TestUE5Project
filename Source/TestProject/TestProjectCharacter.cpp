// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATestProjectCharacter

ATestProjectCharacter::ATestProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
	
	// Set default cube class
	CubeClass = AASpawnCubeActor::StaticClass();
	
	// Set default bullet class
	BulletClass = AABulletActor::StaticClass();
}

void ATestProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATestProjectCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATestProjectCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestProjectCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestProjectCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATestProjectCharacter::LookInput);
		
		// Firing
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATestProjectCharacter::FireInput);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ATestProjectCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ATestProjectCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ATestProjectCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATestProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);

		// print movement speed to the log
		float CurrentSpeed = GetVelocity().Size();
		UE_LOG(LogTemplateCharacter, Log, TEXT("Current Speed: %.2f"), CurrentSpeed);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				0,
				0.05f,
				FColor::Green,
				FString::Printf(TEXT("Current Speed: %.2f"), CurrentSpeed)
			);
		}
	}
}

void ATestProjectCharacter::DoJumpStart()
{
	if (CubeClass)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 500.0f + FVector(0, 0, 300.0f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AASpawnCubeActor>(CubeClass, SpawnLocation, SpawnRotation, SpawnParams);
	}

	// pass Jump to the character
	Jump();
}

void ATestProjectCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ATestProjectCharacter::FireInput(const FInputActionValue& Value)
{
    DoFire();
}

void ATestProjectCharacter::DoFire()
{
    if (BulletClass)
    {
        FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation();
        FRotator SpawnRotation = FirstPersonCameraComponent->GetComponentRotation();
        
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = this;
        
        GetWorld()->SpawnActor<AABulletActor>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
}
