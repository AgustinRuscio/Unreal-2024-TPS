//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "Pistol.h"

#include "Shooter/Components/ShootComponent.h"

//---------------------------------------------------------------------------------------------------------------------------------------
APistol::APistol()
{
 	PrimaryActorTick.bCanEverTick = true;

	bIsAutomatic	 = false;
	MaxAmmoToSave	 = 300;
	MaxAmmoInCharger = 6;
	FireRate		 = 0.0f;
	WeaponType		 = EWeaponType::Pistol;

	ShootComponent->SetCurrentWeaponType(WeaponType);
}