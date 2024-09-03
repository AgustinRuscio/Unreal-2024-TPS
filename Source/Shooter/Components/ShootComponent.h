//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shooter/EnumContainer.h"
#include "ShootComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	UShootComponent();

	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	void SetShootVectors(FVector StartLocation, FVector ForwardLocation);

	UFUNCTION(BlueprintCallable)
	void SetWeaponSkeleton(USkeletalMeshComponent* WeaponSkeletonComp, UAnimSequence* GunShootAnim);
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentWeaponType(EWeaponType NewType);
	
	void FireWeapon();

private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	bool bIsEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	int AmountOfShoots;

	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float ShootSpread;
	
	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float FireRate;
	
	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float GunMaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float GunBaseDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float GunImpulse;

	FVector StartShootPoint;
	FVector ForwardShootVector;

	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	EWeaponType CurrentType;

	TArray<AActor*> IgnoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UCameraShakeBase> ShootCameraShake;
	
	UAnimSequence* WeaponShootAnim;

	FTimerHandle BurstShootingTimerHandle;
	FTimerDelegate BurstShootingTimerDelegate;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	float CalculateDamage(float Distance, FName BoneHitName, class IIDamageable* OtherActor);
	
	FVector CalculateGunSpread(const FVector& Forward) const;
	FVector CalculateEndLocation() const;

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void SetEnemyValues();
	
	void FireSingleBullet();

	void FireBurstBullet();

	void FireMultipleBullets();

	void CheckHit(FHitResult HitResult);

	void ShootFeedBack();
};