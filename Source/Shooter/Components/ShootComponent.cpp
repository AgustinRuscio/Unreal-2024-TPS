//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "ShootComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Shooter/Interface/IDamageable.h"
#include "Shooter/Player/TPS_PlayerController.h"

static int8 ShootingCounter;

//---------------------------------------------------------------------------------------------------------------------------------------
UShootComponent::UShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsEnemy		= false;
	AmountOfShoots	= 6;
	ShootSpread		= 10.f;
	FireRate		= 0.1f;
	GunMaxDistance	= 2000.f;
	GunBaseDamage	= 20.f;
	GunImpulse		= 10000.f;
	WeaponMesh		= nullptr;
	WeaponShootAnim = nullptr;
	CurrentType		= EWeaponType::Pistol;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::SetShootVectors(FVector StartLocation, FVector ForwardLocation)
{
	StartShootPoint	   = StartLocation;
	ForwardShootVector = ForwardLocation;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::SetWeaponSkeleton(USkeletalMeshComponent* WeaponSkeletonComp, UAnimSequence* GunShootAnim)
{
	WeaponMesh		= WeaponSkeletonComp;
	WeaponShootAnim = GunShootAnim;
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
			FireSingleBullet();
				break;
		
		case EWeaponType::Rifle:
			bIsEnemy ? FireBurstBullet() : FireSingleBullet();
				break;

		case EWeaponType::Shotgun:
			FireMultipleBullets();
				break;	
	}

	if(!bIsEnemy)
			UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetOwner()->GetActorLocation(), 1, GetOwner(), 10000.f, "NONE");
}

//---------------------------------------------------------------------------------------------------------------------------------------
float UShootComponent::CalculateDamage(float Distance, FName BoneHitName, IIDamageable* OtherActor)
{
	float damageMultiplier = 1.0f;
	
	if(Distance > GunMaxDistance * 0.75f)
	{
		damageMultiplier -= (Distance / GunMaxDistance);
	}

	if(BoneHitName == OtherActor->GetHeadBone())
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
	
	if(bIsEnemy)
		SetEnemyValues();
	
	IgnoredActors.Add(GetOwner());
	
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_WorldDynamic));
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if(GetWorld())
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(BurstShootingTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(BurstShootingTimerHandle);
	}

	if(BurstShootingTimerDelegate.IsBound())
	{
		BurstShootingTimerDelegate.Unbind();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::SetEnemyValues()
{
	StartShootPoint	   = GetOwner()->GetActorLocation();
	ForwardShootVector = GetOwner()->GetActorForwardVector();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::FireSingleBullet()
{
	FHitResult OutHit;

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartShootPoint, CalculateEndLocation(), 1.0f, ObjectTypes,
		false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Red, FColor::Green, 3.0f );

	ShootFeedBack();

	if(!bHit) return;

	CheckHit(OutHit);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::FireBurstBullet()
{
	ShootingCounter = AmountOfShoots;
	
	if(GetWorld()->GetTimerManager().IsTimerActive(BurstShootingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstShootingTimerHandle);
	}
	
	GetWorld()->GetTimerManager().SetTimer(BurstShootingTimerHandle, this, &UShootComponent::ShootBurst, FireRate, true);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::ShootBurst()
{
	if(ShootingCounter <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstShootingTimerHandle);
		return;
	}

	ShootingCounter--;

	if(bIsEnemy)
		SetEnemyValues();

	FHitResult OutHit;

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartShootPoint, CalculateEndLocation(), 1.0f, ObjectTypes,
		false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Red, FColor::Green, 3.0f );

	ShootFeedBack();

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

	ShootFeedBack();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::CheckHit(FHitResult HitResult)
{
	if(IIDamageable* damageable = Cast<IIDamageable>(HitResult.GetActor()))
		damageable->OnHit(CalculateDamage(HitResult.Distance, HitResult.BoneName , damageable), GunImpulse,HitResult.BoneName);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UShootComponent::ShootFeedBack()
{
	WeaponMesh->PlayAnimation(WeaponShootAnim, false);
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShootCameraShake, GetOwner()->GetActorLocation(), 5000, 0);

	auto controller = Cast<ATPS_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	controller->PlayRumbleFeedBack(.85, .2, true, true, true, true);
}