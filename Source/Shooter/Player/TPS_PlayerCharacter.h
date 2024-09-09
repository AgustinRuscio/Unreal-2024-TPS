//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Shooter/EnumContainer.h"
#include "Shooter/Components/HealthComponent.h"
#include "Shooter/Interface/IDamageable.h"
#include "TPS_PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStateAction : uint8
{
	Idle,
	Moving,
	Aiming
};

UCLASS()
class SHOOTER_API ATPS_PlayerCharacter : public ACharacter, public IIDamageable
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SpawnFloorBloodArrow;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UHealthComponent* HealthComponent;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	FORCEINLINE bool GetAimingState() const { return bIsAiming; };
	
	virtual FName GetHeadBone() const override;
	
	virtual void OnHit(float DamageTaken, float ShooImpulse, FName& BoneHit) override;

	UFUNCTION()
	void OnActorDestroyed() override;
	
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
	
	void MeleeAttack();
	
	UFUNCTION(BlueprintCallable)
	void MeleeAttackStart();
	
	UFUNCTION(BlueprintCallable)
	void MeleeAttackEnd();

	void ReloadWeapon();
	
	void Interaction();
	
	void SetInteractable(class ABaseInteractor* NewInteractable);
	void RemoveInteractable();
	void GetWeapon(EWeaponType WeaponType);

	void SwapWeapon(int index);

	void TakeCover();
	
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
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsTakingCover;
	
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
	bool bIsAttacking;

	bool bCanShoot = true;
	bool bCanUnEquip = true;
	bool bCanAim;
	bool bUnarmed;
	
	int CurrentWeaponIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float MeleeAttackDamage;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FName HeadBoneName;
	
	EPlayerStateAction CurrentState;

	class ATPS_PlayerController* controller;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimMontage* MeleeAttackAnimMontage;
	
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
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UCurveFloat* FloatCurveSpringArmLength;

	class ABaseInteractor* CurrentInteractable;
	class ABaseCoverObject* CurrentCoverObject;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UCameraShakeBase> CameraShakeHit;

	UPROPERTY(EditDefaultsOnly, Category = SFX)
	USoundBase* DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UParticleSystem* BloodVFX;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class ADecalActor> BloodFloor;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeWalk;
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeRun;
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeIdle;
	
	FTimerHandle AimingTimerHandle;
	FTimerDelegate AimingTimerDelegate;

	FTimerHandle DeathTimerHandle;
	FTimerDelegate DeathTimerDelegate;
	
	FTimeline TimeLineSpringArmMoving;
	FTimeline TimeLineSpringArmAiming;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaTime) override;

	void CheckForwardTrace();
	
	void CreateWeapons();
	void EquipWeapon();

	void TakeCurrentCover();
	void LeftCurrentCover();

	void UpdateLifeBar();

	void ShowDeathWidget();
	void HeadBob() const;

	void HitFeedBack() const;
	
	void DeathVFX() const;

	UFUNCTION()
	void OnReloadWeapon();
	
	void BindTimeLines();
	void TimeLinesTick(float DeltaSeconds);

	UFUNCTION()
	void SetSpringArmMovingSettings(float deltaSeconds) const;

	UFUNCTION()
	void SetSpringArmAimingSettings(float deltaSeconds) const;
};