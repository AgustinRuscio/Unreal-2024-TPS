//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "TPS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_PlayerCharacter.h"

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
		
		EnhancedComponent->BindAction(InputActionAim, ETriggerEvent::Started, this, &ATPS_PlayerController::AimStart);
		EnhancedComponent->BindAction(InputActionAim, ETriggerEvent::Completed, this, &ATPS_PlayerController::AimEnd);
		EnhancedComponent->BindAction(InputActionUnEquip, ETriggerEvent::Started, this, &ATPS_PlayerController::UnEquipStart);
		
		EnhancedComponent->BindAction(InputActionShoot, ETriggerEvent::Triggered, this, &ATPS_PlayerController::ShootStart);
		EnhancedComponent->BindAction(InputActionShoot, ETriggerEvent::Completed, this, &ATPS_PlayerController::ShootEnd);
		EnhancedComponent->BindAction(InputActionReload, ETriggerEvent::Started, this, &ATPS_PlayerController::Reload);
		
		EnhancedComponent->BindAction(InputActionMeleeAttack, ETriggerEvent::Started, this, &ATPS_PlayerController::MeleeAttackStart);
		
		EnhancedComponent->BindAction(InputActionInteract, ETriggerEvent::Started, this, &ATPS_PlayerController::InteractStart);
		
		EnhancedComponent->BindAction(InputActionFirstWeapon, ETriggerEvent::Started, this, &ATPS_PlayerController::FirstWeapon);
		EnhancedComponent->BindAction(InputActionSecondWeapon, ETriggerEvent::Started, this, &ATPS_PlayerController::SecondWeapon);
		EnhancedComponent->BindAction(InputActionThirdWeapon, ETriggerEvent::Started, this, &ATPS_PlayerController::ThirdWeapon);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::MovePlayerCharacter(const FInputActionValue& value)
{
	FVector2d MovingAxis = value.Get<FVector2d>();
	PlayerCharacterRef->MovePlayer(MovingAxis);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::MovementStart(const FInputActionValue& value)
{
	PlayerCharacterRef->MovementStart();
}

//---------------------------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::AimStart(const FInputActionValue& value)
{
	PlayerCharacterRef->AimStart();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::AimEnd(const FInputActionValue& value)
{
	PlayerCharacterRef->AimEnd();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::UnEquipStart(const FInputActionValue& value)
{
	PlayerCharacterRef->UnEquipWeapon();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::ShootStart(const FInputActionValue& value)
{
	PlayerCharacterRef->ShootStart();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::ShootEnd(const FInputActionValue& value)
{
	PlayerCharacterRef->ShootEnd();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::Reload(const FInputActionValue& value)
{
	PlayerCharacterRef->ReloadWeapon();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::MeleeAttackStart(const FInputActionValue& value)
{
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::InteractStart(const FInputActionValue& value)
{
	PlayerCharacterRef->Interaction();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::FirstWeapon(const FInputActionValue& value)
{
	PlayerCharacterRef->SwapWeapon(1);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::SecondWeapon(const FInputActionValue& value)
{
	PlayerCharacterRef->SwapWeapon(2);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerController::ThirdWeapon(const FInputActionValue& value)
{
	PlayerCharacterRef->SwapWeapon(3);
}