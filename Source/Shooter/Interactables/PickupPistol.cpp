//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "PickupPistol.h"

#include "Shooter/Player/TPS_PlayerCharacter.h"

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