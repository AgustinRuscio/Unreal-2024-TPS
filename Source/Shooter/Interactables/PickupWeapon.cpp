//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "PickupWeapon.h"

#include "Shooter/Player/TPS_PlayerCharacter.h"
#include "Shooter/Widgets/GunPickupHUD.h"

//---------------------------------------------------------------------------------------------------------------------------------------
APickupWeapon::APickupWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void APickupWeapon::OnInteraction()
{
	Super::OnInteraction();
	Player->GetWeapon(ThisWeaponType);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void APickupWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickupHUD = Cast<UGunPickupHUD>(InteractionWidget);
	PickupHUD->SetWeaponType(ThisWeaponType);
}