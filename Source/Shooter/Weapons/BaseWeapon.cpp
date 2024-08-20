//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseWeapon.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/Widgets/WeaponHUD.h"

static bool bCanSound = true;

//---------------------------------------------------------------------------------------------------------------------------------------
ABaseWeapon::ABaseWeapon()
{
 	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	
	bCanFire = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::OnAim()
{
	WeaponHUD->AddToViewport(2);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::OnAimEnd()
{
	WeaponHUD->RemoveFromParent();
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

	WeaponHUD->UpdateAmmo(this);
	
	SkeletalMeshComponent->PlayAnimation(ShootAnim, false);
	
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShootCameraShake, GetActorLocation(), 5000, 0);
	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
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
	if(AmmoStorage <= 0) return;
	if(CurrentAmmo == MaxAmmoInCharger) return;

	if(AmmoStorage >= MaxAmmoInCharger)
	{
		AmmoStorage -= (MaxAmmoInCharger - CurrentAmmo);
		CurrentAmmo = MaxAmmoInCharger;
	}
	else
	{
		if((CurrentAmmo + AmmoStorage) > MaxAmmoInCharger)
		{
			AmmoStorage = (CurrentAmmo + AmmoStorage) - MaxAmmoInCharger;
			CurrentAmmo = MaxAmmoInCharger;
		}
		else
		{
			CurrentAmmo = (CurrentAmmo + AmmoStorage);
			AmmoStorage = 0;
		}
	}

	WeaponHUD->UpdateAmmo(this);
	SkeletalMeshComponent->PlayAnimation(ReloadAnim, false);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::AddAmmo(int amount)
{
	AmmoStorage += amount;

	AmmoStorage = FMath::Clamp(AmmoStorage, 0, MaxAmmoToSave);
	
	WeaponHUD->UpdateAmmo(this);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseWeapon::AddAmmo()
{
	AmmoStorage += FMath::RandRange(1, 10);

	AmmoStorage = FMath::Clamp(AmmoStorage, 0, MaxAmmoToSave);
	
	WeaponHUD->UpdateAmmo(this);
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
	
	CurrentAmmo = FMath::RandRange(MaxAmmoInCharger / 2, MaxAmmoInCharger);
	AmmoStorage = FMath::RandRange(MaxAmmoToSave / 5, MaxAmmoToSave / 2);
	
	FireRateTimerDelegate.BindLambda([&]
	{
		bCanFire = true;
	});

	NoBulletsTimerDelegate.BindLambda([]
	{
		bCanSound = true;
	});

	WeaponHUD = CreateWidget<UWeaponHUD>(GetWorld(), WeaponWidget);
	WeaponHUD->SetWeaponType(WeaponType);
	WeaponHUD->UpdateAmmo(this);
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