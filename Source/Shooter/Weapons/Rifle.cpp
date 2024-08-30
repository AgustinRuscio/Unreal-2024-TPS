//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "Rifle.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ARifle::ARifle()
{
 	PrimaryActorTick.bCanEverTick = true;

	bIsAutomatic	 = false;
	MaxAmmoToSave	 = 100;
	MaxAmmoInCharger = 30;
	FireRate		 = 0.1f;
	WeaponType		 = EWeaponType::Rifle;
}

void ARifle::FireWeapon (FVector StartShootPoint, FVector ForwardShootPoint)
{
	Super::FireWeapon(StartShootPoint, ForwardShootPoint);
}