//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "TPS_PlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Shooter/Interactables/BaseInteractor.h"
#include "Shooter/Weapons/Pistol.h"
#include "Shooter/Widgets/CrosshairHUD.h"
#include "Shooter/EnumContainer.h"

static float CurrentSpringArmLength;
static FVector CurrentSpringArmSocketOffset;

//---------------------------------------------------------------------------------------------------------------------------------------
ATPS_PlayerCharacter::ATPS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->TargetArmLength = 125.0f;
	SpringArmComp->SocketOffset    = FVector(0.0f, 75.0f, 20.0f);
	SpringArmComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    SpringArmComp->bUsePawnControlRotation = true;	
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed			  = 250.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComp);

	GetMesh()->bReceivesDecals = false;
	bUseControllerRotationYaw  = false;
	
	bCanMove	   = true;
	bCanMoveCamera = true;
	bIsSprinting   = false;
	bUnarmed	   = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovePlayer(FVector2d Direction)
{
	if(!bCanMove) return;
	
	if(Direction.Y != 0)
	{
		const FVector& CameraForward = CameraComponent->GetForwardVector();
		AddMovementInput(CameraForward, Direction.Y);
	}
	
	if(Direction.X != 0)
	{
		const FVector& CameraRight = CameraComponent->GetRightVector();
		AddMovementInput(CameraRight, Direction.X);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovementStart()
{
	if(!bCanMove) return;

	bIsMoving = true;
	
	if(!bIsAiming)
	{
		if(TimeLineSpringArmMoving.IsPlaying())
		{
			TimeLineSpringArmMoving.Play();
		}
		else
		{
			TimeLineSpringArmMoving.PlayFromStart();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovementEnd()
{
	if(!bCanMove) return;
	
	bIsMoving = false;
	
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	if(!bIsAiming)
	{
		if(TimeLineSpringArmMoving.IsPlaying())
		{
			TimeLineSpringArmMoving.Reverse();
		}
		else
		{
			TimeLineSpringArmMoving.ReverseFromEnd();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SprintStart()
{
	if(!bCanMove || bIsAiming) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SprintEnd()
{
	if(!bCanMove|| bIsAiming) return;
	
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 375.0f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::RotateCamera(FVector2d Direction)
{
	if(!bCanMoveCamera) return;
	
	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::AimStart()
{
	if(bUnarmed) return;
	if(CurrentWeapon == nullptr) return;
	
	bIsAiming				  = true;
	bUseControllerRotationYaw = true;
	
	if(TimeLineSpringArmMoving.IsPlaying())
	{
		TimeLineSpringArmMoving.Stop();
	}
	
	CrosshairHUD->OnAimStart();
	
	PlayAnimMontage(CurrentWeapon->GetAimAnimMontage());
	CurrentWeapon->OnAim();
	
	CurrentSpringArmLength		 = SpringArmComp->TargetArmLength;
	CurrentSpringArmSocketOffset = SpringArmComp->SocketOffset;
	
	if(TimeLineSpringArmAiming.IsPlaying())
	{
		TimeLineSpringArmAiming.Play();
	}
	else
	{
		TimeLineSpringArmAiming.PlayFromStart();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::AimEnd()
{
	if(bUnarmed) return;
	
	bIsAiming				  = false;
	bUseControllerRotationYaw = false;
	
	CrosshairHUD->OnAimEnd();
	
	StopAnimMontage(CurrentWeapon->GetAimAnimMontage());
	CurrentWeapon->OnAimEnd();
	
	if(TimeLineSpringArmMoving.IsPlaying())
	{
		TimeLineSpringArmMoving.Stop();
	}

	CurrentSpringArmLength		 = SpringArmComp->TargetArmLength;
	CurrentSpringArmSocketOffset = SpringArmComp->SocketOffset;
	
	if(TimeLineSpringArmAiming.IsPlaying())
	{
		TimeLineSpringArmAiming.Play();
	}
	else
	{
		TimeLineSpringArmAiming.PlayFromStart();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::UnEquipWeapon()
{
	if(CurrentWeapon == nullptr || !bCanUnEquip) return;
	
	PlayAnimMontage(CurrentWeapon->GetEquipAnimMontage(), 1);	
	CurrentWeapon->SetWeaponActive(false);

	AimEnd();

	CurrentWeaponIndex = 0;
	bUnarmed	  = true;
	CurrentWeapon = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::ShootStart()
{
	if(bUnarmed || !bIsAiming || !bCanShoot) return;
	
	CurrentWeapon->FireWeapon();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::ShootEnd()
{
	Pistol->FireEnd();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::ReloadWeapon()
{
	if(CurrentWeapon == nullptr) return;
	if(!bIsAiming) return;
	if(bUnarmed) return;
	if(!CurrentWeapon->CanReload()) return;
	
	CurrentWeapon->Reload();
	
	auto t = PlayAnimMontage(CurrentWeapon->GetReloadAnimMontage());

	bCanShoot   = false;
	bCanUnEquip = false;

	if(!GetWorldTimerManager().IsTimerActive(AnimTimerHandle))
	{
		Del.BindLambda([&]
		{
			if(bIsAiming)
				PlayAnimMontage(CurrentWeapon->GetAimAnimMontage());

			bCanShoot   = true;
			bCanUnEquip = true;
		});
			
		GetWorldTimerManager().SetTimer(AnimTimerHandle,Del , t,false);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::Interaction()
{
	if(CurrentInteractor == nullptr) return;
	
	CurrentInteractor->OnInteraction();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetInteractable(ABaseInteractor* NewInteractable)
{
	if(CurrentInteractor != nullptr && CurrentInteractor != NewInteractable)
	{
		CurrentInteractor->RemoveFromCurrent();
		CurrentInteractor = nullptr;
	}
	
	CurrentInteractor = NewInteractable;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::RemoveInteractable()
{
	if(CurrentInteractor != nullptr)
		CurrentInteractor = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::GetWeapon(EWeaponType WeaponType)
{
	bUnarmed = false;
	
	if(WeaponType == EWeaponType::Pistol)
	{
		if(!bPistolUnlocked)
		{
			bPistolUnlocked	   = true;
			CurrentWeapon	   = Pistol;
			CurrentWeaponIndex = 1;
			
			CrosshairHUD->WeaponUnlocked(CurrentWeapon->GetWeaponType());
		}
		else
		{
			Pistol->AddAmmo();
		}
	}

	if(WeaponType == EWeaponType::Rifle)
	{
		if(!bRifleUnlocked)
		{
			bRifleUnlocked	   = true;
			CurrentWeapon	   = Rifle;
			CurrentWeaponIndex = 2;
			
			CrosshairHUD->WeaponUnlocked(CurrentWeapon->GetWeaponType());
		}
		else
		{
			Rifle->AddAmmo();
		}
	}

	if(WeaponType == EWeaponType::Shotgun)
	{
		if(!bShotgunUnlocked)
		{
			bShotgunUnlocked   = true;
			CurrentWeapon	   = Shotgun;
			CurrentWeaponIndex = 3;
			
			CrosshairHUD->WeaponUnlocked(CurrentWeapon->GetWeaponType());
		}
		else
		{
			Shotgun->AddAmmo();
		}
	}
	
	EquipWeapon();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SwapWeapon(int index)
{
	if(CurrentWeaponIndex == index) return;
	
	if(index == 1)
	{
		if(!bPistolUnlocked) return;
		
		CurrentWeapon	   = Pistol;
		CurrentWeaponIndex = index;
		
		EquipWeapon();
	}
	
	 if(index == 2)
	 {
		if(!bRifleUnlocked) return;
		 
		CurrentWeapon	   = Rifle;
		CurrentWeaponIndex = index;
		bUnarmed		   = false;
	 	
		EquipWeapon();
	 }

	if(index == 3)
	{
		if(!bShotgunUnlocked) return;

		CurrentWeapon	   = Shotgun;
		CurrentWeaponIndex = index;
		bUnarmed		   = false;
		
		EquipWeapon();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CrosshairHUD = CreateWidget<UCrosshairHUD>(GetWorld(), CrosshairWidget);
	CrosshairHUD->AddToViewport();
	
	BindTimeLines();
	CreateWeapons();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerHandle);
	}

	if(Del.IsBound())
	{
		Del.Unbind();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLinesTick(DeltaTime);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::CreateWeapons()
{
	FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
	
	FActorSpawnParameters PistolSpawnParameter;
	PistolSpawnParameter.Name = "Pistol";
	PistolSpawnParameter.Owner = this;
	Pistol = GetWorld()->SpawnActor<ABaseWeapon>(PistolBase, GetActorLocation(), GetActorRotation(), PistolSpawnParameter);
	Pistol->AttachToComponent(GetMesh(), AttachmentRules, "PistolSocket");
	Pistol->SetWeaponActive(false);

	FActorSpawnParameters RifleSpawnParameter;
	RifleSpawnParameter.Name = "Rifle";
	RifleSpawnParameter.Owner = this;
	Rifle = GetWorld()->SpawnActor<ABaseWeapon>(RifleBase, GetActorLocation(), GetActorRotation(), RifleSpawnParameter);
	Rifle->AttachToComponent(GetMesh(), AttachmentRules, "BigGunSocket");
	Rifle->SetWeaponActive(false);

	FActorSpawnParameters ShotgunSpawnParameter;
	ShotgunSpawnParameter.Name = "Shotgun";
	ShotgunSpawnParameter.Owner = this;
	Shotgun = GetWorld()->SpawnActor<ABaseWeapon>(ShotgunBase, GetActorLocation(), GetActorRotation(), ShotgunSpawnParameter);
	Shotgun->AttachToComponent(GetMesh(), AttachmentRules, "BigGunSocket");
	Shotgun->SetWeaponActive(false);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::EquipWeapon()
{
	if(LastWeapon != nullptr && LastWeapon != CurrentWeapon)
	{
		LastWeapon->SetWeaponActive(false);
	}

	LastWeapon = CurrentWeapon;
	CurrentWeapon->SetWeaponActive(true);
	CrosshairHUD->ChangeEquippedWeapon(CurrentWeapon->GetWeaponType());

	auto t = PlayAnimMontage(CurrentWeapon->GetEquipAnimMontage());
	
	bCanShoot   = false;
	bCanUnEquip = false;

	if(!GetWorldTimerManager().IsTimerActive(AnimTimerHandle))
	{
		Del.BindLambda([&]
		{
			if(bIsAiming)
				PlayAnimMontage(CurrentWeapon->GetAimAnimMontage());

			bCanShoot   = true;
			bCanUnEquip = true;
		});
			
		GetWorldTimerManager().SetTimer(AnimTimerHandle,Del , t,false);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BindTimeLines()
{
	//--- Spring Arm Length Moving
	FOnTimelineFloat SpringArmTargetArmLengthMovingChangedTick;
	SpringArmTargetArmLengthMovingChangedTick.BindUFunction(this, FName("SetSpringArmMovingSettings"));
	TimeLineSpringArmMoving.AddInterpFloat(FloatCurveSpringArmLength, SpringArmTargetArmLengthMovingChangedTick);

	//--- Spring Arm Length Aiming
	FOnTimelineFloat SpringArmTargetArmLengthAimingChangedTick;
	SpringArmTargetArmLengthAimingChangedTick.BindUFunction(this, FName("SetSpringArmAimingSettings"));
	TimeLineSpringArmAiming.AddInterpFloat(FloatCurveSpringArmLength, SpringArmTargetArmLengthAimingChangedTick);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::TimeLinesTick(float DeltaSeconds)
{
	TimeLineSpringArmMoving.TickTimeline(DeltaSeconds);
	TimeLineSpringArmAiming.TickTimeline(DeltaSeconds);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetSpringArmMovingSettings(float deltaSeconds) const
{
	auto NewLength = FMath::Lerp(125.0f, 200.0f, deltaSeconds);
	SpringArmComp->TargetArmLength = NewLength;
	
	auto NewLSocketOffset = FMath::Lerp(FVector(0.0f, 75.0f, 20.0f), FVector(0.0f, 90.0f, 30.0f), deltaSeconds);
	SpringArmComp->SocketOffset			= NewLSocketOffset;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetSpringArmAimingSettings(float deltaSeconds) const
{
	if(bIsAiming)
	{
		auto NewLength = FMath::Lerp(CurrentSpringArmLength, 215.0f, deltaSeconds);
		SpringArmComp->TargetArmLength = NewLength;
		
		auto NewLSocketOffset = FMath::Lerp(CurrentSpringArmSocketOffset, FVector(0.0f, 90.0f, 30.0f), deltaSeconds);
		SpringArmComp->SocketOffset			= NewLSocketOffset;
	}
	else
	{
		if(bIsMoving)
		{
			auto NewLength = FMath::Lerp(CurrentSpringArmLength, 200.0f, deltaSeconds);
			SpringArmComp->TargetArmLength = NewLength;
		
			auto NewLSocketOffset = FMath::Lerp(CurrentSpringArmSocketOffset, FVector(0.0f, 90.0f, 30.0f), deltaSeconds);
			SpringArmComp->SocketOffset			= NewLSocketOffset;
		}
		else
		{
			auto NewLength = FMath::Lerp(CurrentSpringArmLength, 125.0f, deltaSeconds);
			SpringArmComp->TargetArmLength = NewLength;
		
			auto NewLSocketOffset = FMath::Lerp(CurrentSpringArmSocketOffset, FVector(0.0f, 75.0f, 20.0f), deltaSeconds);
			SpringArmComp->SocketOffset			= NewLSocketOffset;
		}
	}
}