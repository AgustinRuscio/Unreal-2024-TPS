//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Shooter/EnumContainer.h"
#include "TPS_PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStateAction : uint8
{
	Idle,
	Moving,
	Aiming
};

UCLASS()
class SHOOTER_API ATPS_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ATPS_PlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	class UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TSubclassOf<class UCameraShakeBase> CameraShakeHit;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
	void MovePlayer(FVector2d Direction);
	void MovementStart();
	void MovementEnd();
	
	void SprintStart();
	void SprintEnd();
	
	void RotateCamera(FVector2d Direction);
	
	void AimStart();
	void AimEnd();
	void UnEquipWeapon();
	
	void ShootStart();
	void ShootEnd();

	void ReloadWeapon();
	
	void Interaction();
	
	void SetInteractable(class ABaseInteractor* NewInteractable);
	void RemoveInteractable();
	void GetWeapon(EWeaponType WeaponType);

	void SwapWeapon(int index);
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//
	UPROPERTY(BlueprintReadOnly, Category = State)
	bool bIsAiming;
	
	UPROPERTY(BlueprintReadOnly)
	bool bPistolUnlocked;

	UPROPERTY(BlueprintReadOnly)
	bool bRifleUnlocked;

	UPROPERTY(BlueprintReadOnly)
	bool bShotgunUnlocked;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//
	
private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							...// 
	//*****************************************************************************//

	bool bCanMove;
	bool bCanMoveCamera;
	bool bIsMoving;
	bool bIsSprinting;

	bool bCanShoot = true;
	bool bCanUnEquip = true;
	bool bCanAim;
	bool bUnarmed;

	int CurrentWeaponIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapons)
	TSubclassOf<class ABaseWeapon> PistolBase;
	class ABaseWeapon* Pistol;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapons)
	TSubclassOf<class ABaseWeapon> RifleBase;
	class ABaseWeapon* Rifle;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapons)
	TSubclassOf<class ABaseWeapon> ShotgunBase;
	class ABaseWeapon* Shotgun;

	class ABaseWeapon* CurrentWeapon;
	class ABaseWeapon* LastWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UCrosshairHUD> CrosshairWidget;
	UCrosshairHUD* CrosshairHUD;
	
	EPlayerStateAction CurrentState;


	FTimerHandle AnimTimerHandle;
	FTimerDelegate Del;
	
	FTimeline TimeLineSpringArmMoving;
	FTimeline TimeLineSpringArmAiming;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UCurveFloat* FloatCurveSpringArmLength;

	class ABaseInteractor* CurrentInteractor;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//
	
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	virtual void Tick(float DeltaTime) override;

	void CreateWeapons();
	void EquipWeapon();

	UFUNCTION()
	void OnReloadWeapon();
	
	void BindTimeLines();
	void TimeLinesTick(float DeltaSeconds);
	
	UFUNCTION()
	void SetSpringArmMovingSettings(float deltaSeconds) const;
	
	UFUNCTION()
	void SetSpringArmAimingSettings(float deltaSeconds) const;
};