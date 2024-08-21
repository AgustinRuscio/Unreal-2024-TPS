//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "PickupPistol.h"

#include "Shooter/Player/TPS_PlayerCharacter.h"
#include "Shooter/Widgets/GunPickupHUD.h"

//---------------------------------------------------------------------------------------------------------------------------------------
APickupPistol::APickupPistol()
{
	PrimaryActorTick.bCanEverTick = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void APickupPistol::OnInteraction()
{
	Super::OnInteraction();
	Player->GetWeapon(ThisWeaponType);
}

void APickupPistol::BeginPlay()
{
	Super::BeginPlay();

	PickupHUD = Cast<UGunPickupHUD>(InteractionWidget);
	PickupHUD->SetWeaponType(ThisWeaponType);
}