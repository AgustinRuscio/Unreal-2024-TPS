//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "Shotgun.h"

#include "Kismet/GameplayStatics.h"
#include "Shooter/Player/TPS_PlayerCharacter.h"

//---------------------------------------------------------------------------------------------------------------------------------------
AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShotgun::FireWeapon()
{
	if(!CanFireCheck()) return;
	if(!BulletsCheck()) return;
	
	Super::FireWeapon();

	int i = AmountOfShoots - 1;
	while (i >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d "), i);
	
		auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FVector Start = CameraManager->GetCameraLocation();
		FVector Forward = CameraManager->GetCameraRotation().Vector();
		
		FVector End = Start + CalculateGunSpread(Forward) * GunMaxDistance;
	
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));
		ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_WorldDynamic));
	
		AActor* Player = Cast<ATPS_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(Player);
		IgnoredActors.Add(this);
	
		FHitResult OutHit;
	
		const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 1.0f, ObjectTypes,
			false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Red, FColor::Green, 3.0f );
	
			CheckHited(OutHit);
		i--;
	}
}