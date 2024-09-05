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
	void OnPlayerDeath();

	void UpdateHealthBar(float BarValue);

	void ShowHideStealthKill(bool Activate);
	
	void PlayRumbleFeedBack(float intensity, float duration, bool LLarge, bool LSmall, bool RLarge, bool RSmall);

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

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionMovePlayer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionSprint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionMoveCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionAim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionShoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionUnEquip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionReload;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionMeleeAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionFirstWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionSecondWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionThirdWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const class UInputAction* InputActionTTakeCover;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<UUserWidget> DeathWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UPlayerHUD> PlayerHUDWidget;

	class UUserWidget* DeathHUD;
	class UPlayerHUD* PlayerHUD;
	
	FTimerHandle DeathTimerHandle;
	FTimerDelegate DeathTimerDelegate;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	virtual void BeginPlay() override;

	void CreatePlayerWidgets();
	
	void UnbindInputs();
	void BindRegularInputs();

	void MovePlayerCharacter(const FInputActionValue& value);
	void MovementStart(const FInputActionValue& value);
	void MovementEnd(const FInputActionValue& value);
	
	void SprintStart(const FInputActionValue& value);
	void SprintEnd(const FInputActionValue& value);
	
	void RotatePlayerCamera(const FInputActionValue& value);

	void AimStart(const FInputActionValue& value);
	void AimEnd(const FInputActionValue& value);
	void UnEquipStart(const FInputActionValue& value);
	
	void ShootStart(const FInputActionValue& value);
	void ShootEnd(const FInputActionValue& value);
	
	void Reload(const FInputActionValue& value);
	void MeleeAttackStart(const FInputActionValue& value);
	
	void InteractStart(const FInputActionValue& value);
	
	void FirstWeapon(const FInputActionValue& value);
	void SecondWeapon(const FInputActionValue& value);
	void ThirdWeapon(const FInputActionValue& value);
	
	void TakeCover(const FInputActionValue& value);
};