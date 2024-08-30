//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "ShootComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Shooter/Interface/IDamageable.h"

//---------------------------------------------------------------------------------------------------------------------------------------
UShootComponent::UShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::SetShootVectors(FVector StartLocation, FVector ForwardLocation)
{
	StartShootPoint	   = StartLocation;
	ForwardShootVector = ForwardLocation;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::SetCurrentWeaponType(EWeaponType NewType)
{
	CurrentType = NewType;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::FireWeapon()
{
	switch (CurrentType)
	{
		case EWeaponType::Pistol:
		case EWeaponType::Rifle:
			FireSingleBullet();
				break;

		case EWeaponType::Shotgun:
			FireMultipleBullets();
				break;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
FVector UShootComponent::CalculateGunSpread(const FVector& Forward) const
{
	return FMath::VRandCone(Forward, FMath::DegreesToRadians(ShootSpread));
}

//---------------------------------------------------------------------------------------------------------------------------------------
FVector UShootComponent::CalculateEndLocation() const
{
	return StartShootPoint + CalculateGunSpread(ForwardShootVector) * GunMaxDistance;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	
	IgnoredActors.Add(GetOwner());

	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_WorldDynamic));
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::FireSingleBullet()
{
	FHitResult OutHit;

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartShootPoint, CalculateEndLocation(), 1.0f, ObjectTypes,
		false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Red, FColor::Green, 3.0f );

	if(!bHit) return;

	CheckHit(OutHit);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::FireMultipleBullets()
{
	int i = 0;

	while (i < AmountOfShoots)
	{
		++i;
		
		FHitResult OutHit;

		const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartShootPoint, CalculateEndLocation(), 1.0f, ObjectTypes,
			false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Red, FColor::Green, 3.0f );

		if(!bHit) continue;

		CheckHit(OutHit);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::CheckHit(FHitResult HitReuslt)
{
	if(IIDamageable* damageable = Cast<IIDamageable>(HitReuslt.GetActor()))
		damageable->OnHit(CalculateDamage(HitReuslt.Distance, HitReuslt.BoneName , damageable), GunImpulse,HitReuslt.BoneName);
}

//---------------------------------------------------------------------------------------------------------------------------------------
float UShootComponent::CalculateDamage(float Distance, FName BoneHittedName, IIDamageable* OtherActor)
{
	float damageMultiplier = 1.0f;
	
	if(Distance > GunMaxDistance * 0.75f)
	{
		damageMultiplier -= (Distance / GunMaxDistance);
	}

	if(BoneHittedName == OtherActor->GetHeadBone())
	{
		damageMultiplier = 100.0f;

		if(Distance> GunMaxDistance * 0.5f)
		{
			damageMultiplier -= (Distance / GunMaxDistance);
		}
	}
	
	damageMultiplier = FMath::Clamp(damageMultiplier, 0.0f, 2.0f);

	float finalDamage = GunBaseDamage * damageMultiplier;

	return finalDamage;
}