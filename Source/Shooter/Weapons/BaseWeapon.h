//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

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

	FORCEINLINE bool IsActivated() const { return bIsActive; };
	
	FORCEINLINE bool IsAutomatic() const { return bIsAutomatic; };
	
	FORCEINLINE UAnimMontage* GetAimAnimMontage() const { return AimAnimMontage; };
	
	virtual void FireWeapon();
	virtual void FireEnd();
	virtual void Reload();

	void SetWeaponActive(bool Activate);
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	bool bIsAutomatic;
	bool bIsActive;
	bool bCanFire;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float MaxAmmo;
	float CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float GunSpread;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float GunMaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float Damage;
	
	FTimerHandle FireRateTimerHandle;
	FTimerDelegate FireRateTimerDelegate;
	
	FTimerHandle NoBulletsTimerHandle;
	FTimerDelegate NoBulletsTimerDelegate;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class USoundBase* NoBulletSound;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UCameraShakeBase> ShootCameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimMontage* AimAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimSequence* ShootAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UAnimSequence* ReloadAnim;
	
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//

	bool CanFireCheck() const;
	bool BulletsCheck();

	FVector CalculateGunSpread(const FVector& Forward) const;

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	virtual void Tick(float DeltaTime) override;
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	void UnbindTimers();
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//
	

};
