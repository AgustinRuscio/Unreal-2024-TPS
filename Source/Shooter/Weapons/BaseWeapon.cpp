//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseWeapon.h"

#include "Kismet/GameplayStatics.h"

static bool bCanSound = true;

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
		if(!GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, FireRateTimerDelegate, FireRate, false);
		}
	}

	--CurrentAmmo;

	//UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	//AnimInstance->PlaySlotAnimationAsDynamicMontage(ShootAnim);

	SkeletalMeshComponent->PlayAnimation(ShootAnim, false);
	
	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShootCameraShake, GetActorLocation(), 0, 1000);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::FireEnd()
{
	if(bIsAutomatic)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		bCanFire = true;
	}
	else
	{
		bCanFire = true;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::Reload()
{
	SkeletalMeshComponent->PlayAnimation(ReloadAnim, false);
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
		return (bCanFire && !GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle));
	}
	else
	{
		return bCanFire;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
bool ABaseWeapon::BulletsCheck()
{
	if(CurrentAmmo <= 0)
	{
		if(bCanSound)
		{
			bCanSound = false;
			
			UGameplayStatics::PlaySound2D(GetWorld(), NoBulletSound);

			if(!GetWorld()->GetTimerManager().IsTimerActive(NoBulletsTimerHandle))
				GetWorld()->GetTimerManager().SetTimer(NoBulletsTimerHandle, NoBulletsTimerDelegate, NoBulletSound->Duration, false);
		}
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

	FireRateTimerDelegate.BindLambda([&]
	{
		bCanFire = true;
	});

	NoBulletsTimerDelegate.BindLambda([]
	{
		bCanSound = true;
	});
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::BeginDestroy()
{
	Super::BeginDestroy();
	
	UnbindTimers();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::UnbindTimers()
{
	if (GetWorld())
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);

		if(GetWorld()->GetTimerManager().IsTimerActive(NoBulletsTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(NoBulletsTimerHandle);
	}

	if (FireRateTimerDelegate.IsBound())
	{
		FireRateTimerDelegate.Unbind();
	}
	
	if (NoBulletsTimerDelegate.IsBound())
	{
		NoBulletsTimerDelegate.Unbind();
	}
}