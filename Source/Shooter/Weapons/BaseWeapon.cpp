//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseWeapon.h"

#include "Kismet/GameplayStatics.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ABaseWeapon::ABaseWeapon()
{
 	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	
	bCanFire = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::FireWeapon()
{
	bCanFire = false;

	if(bIsAutomatic)
	{
		if(!GetWorld()->GetTimerManager().IsTimerActive(FireRateTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(FireRateTimer, FireRateDelegate, FireRate, false);
		}
	}

	--CurrentAmmo;

	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShootCameraShake, GetActorLocation(), 0, 1000);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::FireEnd()
{
	if(bIsAutomatic)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
		bCanFire = true;
	}
	else
	{
		bCanFire = true;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::SetWeaponActive(bool Activate)
{
	bIsActive = Activate;
	SkeletalMeshComponent->SetVisibility(Activate);
}

//---------------------------------------------------------------------------------------------------------------------------------------
bool ABaseWeapon::CanFireCheck() const
{
	if(!bIsActive)
	{
		return false;
	}
	
	if(bIsAutomatic)
	{
		return (bCanFire && !GetWorld()->GetTimerManager().IsTimerActive(FireRateTimer));
	}
	else
	{
		return bCanFire;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
bool ABaseWeapon::BulletsCheck() const
{
	if(CurrentAmmo <= 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), NoBulletSound);
		return false;
	}
	else
		return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
FVector ABaseWeapon::CalculateGunSpread(const FVector& Forward) const
{
	return FMath::VRandCone(Forward, FMath::DegreesToRadians(GunSpread));
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	FireRateDelegate.BindLambda([&]
	{
		bCanFire = true;
	});
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (GetWorld())
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(FireRateTimer))
			GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
	}

	if (FireRateDelegate.IsBound())
	{
		FireRateDelegate.Unbind();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}