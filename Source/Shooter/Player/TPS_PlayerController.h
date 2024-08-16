//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "TPS_PlayerController.generated.h"


UCLASS()
class SHOOTER_API ATPS_PlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ATPS_PlayerController();
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//
	
private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	class ATPS_PlayerCharacter* PlayerCharacterRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionMovePlayer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionSprint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionMoveCamera;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	virtual void BeginPlay() override;

	void UnbindInputs();
	void BindRegularInputs();

	void MovePlayerCharacter(const FInputActionValue& value);
	void MovementStart(const FInputActionValue& value);
	void MovementEnd(const FInputActionValue& value);
	
	void SprintStart(const FInputActionValue& value);
	void SprintEnd(const FInputActionValue& value);
	
	void RotatePlayerCamera(const FInputActionValue& value);
};