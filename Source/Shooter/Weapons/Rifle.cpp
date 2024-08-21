//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "Rifle.h"

#include "Kismet/GameplayStatics.h"
#include "Shooter/Player/TPS_PlayerCharacter.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ARifle::ARifle()
{
 	PrimaryActorTick.bCanEverTick = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ARifle::FireWeapon()
{
	if(!CanFireCheck()) return;

	if(!BulletsCheck()) return;
	
	Super::FireWeapon();
	
	auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector Start = CameraManager->GetCameraLocation();
	FVector Forward = CameraManager->GetCameraRotation().Vector();
	
	FVector End = Start + CalculateGunSpread(Forward) * GunMaxDistance;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));

	AActor* Player = Cast<ATPS_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Player);
	IgnoredActors.Add(this);

	TArray<FHitResult> OutHit;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 1.0f, ObjectTypes,
		false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Red, FColor::Green, 3.0f );

	if(!bHit) return;
}