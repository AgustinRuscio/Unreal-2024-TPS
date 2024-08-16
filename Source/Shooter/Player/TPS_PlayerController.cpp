//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "TPS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPS_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ATPS_PlayerController::ATPS_PlayerController() { }

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacterRef = CastChecked<ATPS_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	BindRegularInputs();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::UnbindInputs()
{
	if(UEnhancedInputComponent* EnhancedComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedComponent->ClearActionBindings();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::BindRegularInputs()
{
	UnbindInputs();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(MappingContext, 0);

	if(UEnhancedInputComponent* EnhancedComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedComponent->BindAction(InputActionMovePlayer, ETriggerEvent::Started, this, &ATPS_PlayerController::MovementStart);
		EnhancedComponent->BindAction(InputActionMovePlayer, ETriggerEvent::Triggered, this, &ATPS_PlayerController::MovePlayerCharacter);
		EnhancedComponent->BindAction(InputActionMovePlayer, ETriggerEvent::Completed, this, &ATPS_PlayerController::MovementEnd);
		
		EnhancedComponent->BindAction(InputActionSprint, ETriggerEvent::Triggered, this, &ATPS_PlayerController::SprintStart);
		EnhancedComponent->BindAction(InputActionSprint, ETriggerEvent::Completed, this, &ATPS_PlayerController::SprintEnd);
		
		EnhancedComponent->BindAction(InputActionMoveCamera, ETriggerEvent::Triggered, this, &ATPS_PlayerController::RotatePlayerCamera);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::MovePlayerCharacter(const FInputActionValue& value)
{
	FVector2d MovingAxis = value.Get<FVector2d>();
	PlayerCharacterRef->MovePlayer(MovingAxis);
}

void ATPS_PlayerController::MovementStart(const FInputActionValue& value)
{
	PlayerCharacterRef->MovementStart();
}

void ATPS_PlayerController::MovementEnd(const FInputActionValue& value)
{
	PlayerCharacterRef->MovementEnd();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::SprintStart(const FInputActionValue& value)
{
	PlayerCharacterRef->SprintStart();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::SprintEnd(const FInputActionValue& value)
{
	PlayerCharacterRef->SprintEnd();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::RotatePlayerCamera(const FInputActionValue& value)
{
	FVector2d RotationAxis = value.Get<FVector2d>();
	PlayerCharacterRef->RotateCamera(RotationAxis);
}