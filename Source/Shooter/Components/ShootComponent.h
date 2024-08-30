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
	void SetCurrentWeaponType(EWeaponType NewType);
	
	void FireWeapon();

private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	int AmountOfShoots;

	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float ShootSpread;
	
	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float GunMaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float GunBaseDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	float GunImpulse;

	FVector StartShootPoint;
	FVector ForwardShootVector;

	UPROPERTY(EditDefaultsOnly, Category = ShootSettings)
	EWeaponType CurrentType;

	TArray<AActor*> IgnoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	FVector CalculateGunSpread(const FVector& Forward) const;
	FVector CalculateEndLocation() const;

	virtual void BeginPlay() override;

	void FireSingleBullet();

	void FireMultipleBullets();

	void CheckHit(FHitResult HitReuslt);

	float CalculateDamage(float Distance, FName BoneHittedName, class IIDamageable* OtherActor);
};