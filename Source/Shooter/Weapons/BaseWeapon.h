//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shooter/EnumContainer.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReload);

UCLASS()
class SHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:

	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ABaseWeapon();

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	FOnReload OnReload;
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	FORCEINLINE bool IsActivated() const { return bIsActive; };
	
	FORCEINLINE bool IsAutomatic() const { return bIsAutomatic; };
	bool CanReload() const;
	
	FORCEINLINE UAnimMontage* GetAimAnimMontage() const { return AimAnimMontage; };
	FORCEINLINE UAnimMontage* GetReloadAnimMontage() const { return ReloadAnimMontage; };
	FORCEINLINE UAnimMontage* GetEquipAnimMontage() const { return EquipAnimMontage; };
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; };


	virtual void OnAim();
	virtual void OnAimEnd();
	
	virtual void FireWeapon(FVector StartShootPoint, FVector ForwardShootPoint);
	virtual void FireEnd();
	virtual void Reload();

	void AddAmmo(int amount);
	void AddAmmo();
	
	void SetWeaponActive(bool Activate);
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	bool bIsAutomatic;
	bool bIsActive;
	bool bCanFire;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	int MaxAmmoToSave;
	
	UPROPERTY(BlueprintReadOnly, Category = Settings)
	int AmmoStorage;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	int MaxAmmoInCharger;
	
	UPROPERTY(BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float FireRate;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly)
	class UShootComponent* ShootComponent;
	
	FTimerHandle FireRateTimerHandle;
	FTimerDelegate FireRateTimerDelegate;
	
	FTimerHandle NoBulletsTimerHandle;
	FTimerDelegate NoBulletsTimerDelegate;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class USoundBase* NoBulletSound;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UWeaponHUD> WeaponWidget;
	class UWeaponHUD* WeaponHUD;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UCameraShakeBase> ShootCameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimMontage* AimAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimMontage* ReloadAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimMontage* EquipAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimSequence* GunShootAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimSequence* ReloadAnim;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//

	bool CanFireCheck() const;
	bool BulletsCheck();
	
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	void UnbindTimers();
};