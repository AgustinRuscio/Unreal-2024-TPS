//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "TPS_PlayerCharacter.h"

#include "TPS_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DecalActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/Interactables/BaseInteractor.h"
#include "Shooter/Weapons/Pistol.h"
#include "Shooter/Widgets/CrosshairHUD.h"
#include "Shooter/EnumContainer.h"
#include "Shooter/Enemies/BaseEnemy.h"
#include "Shooter/EnvironmentActors/BaseCoverObject.h"

static float CurrentSpringArmLength;
static float CurrentShootImpulse;
static float MeleeAttackDistance = 100.f;
static FVector CurrentSpringArmSocketOffset;
static FName CurrentHitBoneName;
static ABaseEnemy* EnemyForInstaKill;

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
	
	SpawnFloorBloodArrow = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	SpawnFloorBloodArrow->SetupAttachment(GetMesh(), "head");
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComp, "head");
	GetMesh()->bReceivesDecals = false;
	bUseControllerRotationYaw  = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddDynamic(this, &ATPS_PlayerCharacter::OnActorDestroyed);
	
	bCanMove	   = true;
	bCanMoveCamera = true;
	bIsSprinting   = false;
	bUnarmed	   = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
FName ATPS_PlayerCharacter::GetHeadBone() const
{
	return HeadBoneName;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::OnHit(float DamageTaken, float ShooImpulse, FName& BoneHit)
{
	HealthComponent->TakeDamage(DamageTaken);
	CurrentShootImpulse   = ShooImpulse;
	CurrentHitBoneName	  = BoneHit;

	UpdateLifeBar();
	HitFeedBack();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::OnActorDestroyed()
{
	DeathVFX();

	TimeLineSpringArmMoving.PlayFromStart();
	
	bCanMove = false;
	bCanShoot = false;
	
	ShowDeathWidget();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovePlayer(FVector2d Direction)
{
	if(!bCanMove) return;
	
	if(Direction.Y != 0 && !bIsTakingCover)
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
	
	CurrentWeapon->FireWeapon(CameraComponent->GetComponentLocation(), CameraComponent->GetComponentRotation().Vector());
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::ShootEnd()
{
	Pistol->FireEnd();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MeleeAttack()
{
	if(bIsAttacking) return;
	
	PlayAnimMontage(MeleeAttackAnimMontage);
	bCanShoot	 = false;
	bCanAim		 = false;
	bIsAttacking = true;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MeleeAttackStart()
{
	if(EnemyForInstaKill != nullptr)
	{
		EnemyForInstaKill->InstaKill();
	}
	else
	{
		TArray<FHitResult> HitResults;

		const FVector& StartBox = GetActorLocation();
		const FVector& EndBox   = StartBox + (GetActorForwardVector() * MeleeAttackDistance);
		const FVector& BoxSize  = FVector(20.0f, 50.0f, 50.0f);

		TArray<AActor*> IgnoreTheseActors;
		IgnoreTheseActors.Add(this);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));

		const bool bBoxHits = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartBox, EndBox, BoxSize,
		GetActorRotation(), ObjectTypes, true, IgnoreTheseActors, EDrawDebugTrace::ForDuration, HitResults, true, 
		FColor::Red, FColor::Green, 3.0f);

		if (bBoxHits)
		{
			bool bHitAnEnemy = false;

			TArray<IIDamageable*> HitEnemies;
			
			for (const auto& CurrentHit : HitResults)
			{
				if (IIDamageable* TempEnemy = Cast<IIDamageable>(CurrentHit.GetActor()))
				{
					if(HitEnemies.Contains(TempEnemy)) continue;
					
					FName a = "NONE";
					HitEnemies.Add(TempEnemy);
					TempEnemy->OnHit(MeleeAttackDamage, 10000.f,a);
					bHitAnEnemy = true;
				}
			}

			if (bHitAnEnemy)
			{
				HitFeedBack();
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MeleeAttackEnd()
{
	bCanShoot	 = true;
	bCanAim		 = true;
	bIsAttacking = false;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::ReloadWeapon()
{
	if(CurrentWeapon == nullptr) return;
	if(!bIsAiming) return;
	if(bUnarmed) return;
	if(!CurrentWeapon->CanReload()) return;
	
	CurrentWeapon->Reload();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::Interaction()
{
	if(CurrentInteractable == nullptr) return;
	
	CurrentInteractable->OnInteraction();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetInteractable(ABaseInteractor* NewInteractable)
{
	if(CurrentInteractable != nullptr && CurrentInteractable != NewInteractable)
	{
		CurrentInteractable->RemoveFromCurrent();
		CurrentInteractable = nullptr;
	}
	
	CurrentInteractable = NewInteractable;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::RemoveInteractable()
{
	if(CurrentInteractable != nullptr)
		CurrentInteractable = nullptr;
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
void ATPS_PlayerCharacter::TakeCover()
{
	if(bIsTakingCover)
		LeftCurrentCover();
	else
		TakeCurrentCover();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetCoverObject(ABaseCoverObject* currentCover)
{
	CurrentCoverObject = currentCover;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	controller = CastChecked<ATPS_PlayerController>(Controller);
	
	CrosshairHUD = CreateWidget<UCrosshairHUD>(GetWorld(), CrosshairWidget);
	CrosshairHUD->AddToViewport();
	
	BindTimeLines();
	CreateWeapons();
	
	UpdateLifeBar();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	if(GetWorld())
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(AimingTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
		
		if(GetWorld()->GetTimerManager().IsTimerActive(DeathTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	}

	if(AimingTimerDelegate.IsBound())
	{
		AimingTimerDelegate.Unbind();
	}
	
	if(DeathTimerDelegate.IsBound())
	{
		DeathTimerDelegate.Unbind();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForwardTrace();
	
	TimeLinesTick(DeltaTime);

	HeadBob();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::CheckForwardTrace()
{
	FVector StartShootPoint = GetActorLocation();
	FVector EndShootPoint = StartShootPoint + (GetActorRotation().Vector() * 100);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_WorldDynamic));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	
	FHitResult OutHit;
	
	bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartShootPoint, EndShootPoint, 15.0f, ObjectTypes,
	   false, IgnoredActors, EDrawDebugTrace::ForOneFrame, OutHit, true, FColor::Red, FColor::Green, 3.0f );
	
	if(!hit)
	{
		EnemyForInstaKill = nullptr;
		controller->ShowHideStealthKill(false);
	}
	else
	{
		if(ABaseEnemy* enemy = Cast<ABaseEnemy>(OutHit.GetActor()))
		{
			if(!enemy->GetIsAwareOfPlayerPresence())
			{
				EnemyForInstaKill = enemy;
				controller->ShowHideStealthKill(true);
			}
			else
			{
				EnemyForInstaKill = nullptr;
				controller->ShowHideStealthKill(false);
			}
		}
	}

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
	Pistol->OnReload.AddDynamic(this, &ATPS_PlayerCharacter::OnReloadWeapon);
	Pistol->SetWeaponActive(false);

	FActorSpawnParameters RifleSpawnParameter;
	RifleSpawnParameter.Name = "Rifle";
	RifleSpawnParameter.Owner = this;
	Rifle = GetWorld()->SpawnActor<ABaseWeapon>(RifleBase, GetActorLocation(), GetActorRotation(), RifleSpawnParameter);
	Rifle->AttachToComponent(GetMesh(), AttachmentRules, "BigGunSocket");
	Rifle->OnReload.AddDynamic(this, &ATPS_PlayerCharacter::OnReloadWeapon);
	Rifle->SetWeaponActive(false);

	FActorSpawnParameters ShotgunSpawnParameter;
	ShotgunSpawnParameter.Name = "Shotgun";
	ShotgunSpawnParameter.Owner = this;
	Shotgun = GetWorld()->SpawnActor<ABaseWeapon>(ShotgunBase, GetActorLocation(), GetActorRotation(), ShotgunSpawnParameter);
	Shotgun->AttachToComponent(GetMesh(), AttachmentRules, "BigGunSocket");
	Shotgun->OnReload.AddDynamic(this, &ATPS_PlayerCharacter::OnReloadWeapon);
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

	if(!GetWorldTimerManager().IsTimerActive(AimingTimerHandle))
	{
		AimingTimerDelegate.BindLambda([&]
		{
			if(bIsAiming)
				PlayAnimMontage(CurrentWeapon->GetAimAnimMontage());

			bCanShoot   = true;
			bCanUnEquip = true;
		});
			
		GetWorldTimerManager().SetTimer(AimingTimerHandle,AimingTimerDelegate , t,false);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::TakeCurrentCover()
{
	if(CurrentCoverObject == nullptr || bIsTakingCover) return;

	bIsTakingCover							 = true;
	bIsCrouching							 = CurrentCoverObject->GetCrouchCover();
	bUseControllerRotationYaw				 = false;
	CameraComponent->bUsePawnControlRotation = false;
	FaceRotation(CurrentCoverObject->GetCoverRotation());

	SetActorRotation(CurrentCoverObject->GetCoverRotation());
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::LeftCurrentCover()
{
	bUseControllerRotationYaw				 = true;
	bIsCrouching							 = false;
	bIsTakingCover							 = false;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::UpdateLifeBar()
{
	controller->UpdateHealthBar(HealthComponent->GetHealthPercent());
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::ShowDeathWidget()
{
	DeathTimerDelegate.BindLambda([&]
	{
		controller->OnPlayerDeath();
	});

	if(!GetWorld()->GetTimerManager().IsTimerActive(DeathTimerHandle))
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, DeathTimerDelegate, 3.f, false);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::HeadBob() const
{
	controller->PlayRumbleFeedBack(.85, .2, true, true, true, true);
	
	if(GetVelocity().Length() > 0)
	{
		if(GetVelocity().Length() < 600)
			controller->ClientStartCameraShake(CameraShakeWalk);
		else
			controller->ClientStartCameraShake(CameraShakeRun);
	}
	else
		controller->ClientStartCameraShake(CameraShakeIdle);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::HitFeedBack() const
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeHit, GetOwner()->GetActorLocation(), 5000, 0);

	controller->PlayRumbleFeedBack(.85, .2, true, true, true, true);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::DeathVFX() const
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->AddImpulse((GetActorForwardVector() * -1) * CurrentShootImpulse, CurrentHitBoneName, false);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFX, GetActorLocation(), FRotator::ZeroRotator,
						FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);

	GetWorld()->SpawnActor<ADecalActor>(BloodFloor, SpawnFloorBloodArrow->GetComponentLocation(),
										SpawnFloorBloodArrow->GetComponentRotation(), SpawnParams);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::OnReloadWeapon()
{
	auto t = PlayAnimMontage(CurrentWeapon->GetReloadAnimMontage());

	bCanShoot   = false;
	bCanUnEquip = false;

	if(!GetWorldTimerManager().IsTimerActive(AimingTimerHandle))
	{
		AimingTimerDelegate.BindLambda([&]
		{
			if(bIsAiming)
				PlayAnimMontage(CurrentWeapon->GetAimAnimMontage());

			bCanShoot   = true;
			bCanUnEquip = true;
		});
			
		GetWorldTimerManager().SetTimer(AimingTimerHandle,AimingTimerDelegate , t,false);
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