//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "Shotgun.h"

#include "Shooter/Components/ShootComponent.h"

//---------------------------------------------------------------------------------------------------------------------------------------
AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bIsAutomatic	 = true;
	MaxAmmoToSave	 = 30;
	MaxAmmoInCharger = 4;
	FireRate		 = 1.0f;
	WeaponType		 = EWeaponType::Shotgun;
	
	ShootComponent->SetCurrentWeaponType(WeaponType);
	ShootComponent->SetWeaponSkeleton(SkeletalMeshComponent, GunShootAnim);
}