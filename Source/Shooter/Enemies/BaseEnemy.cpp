//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseEnemy.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DecalActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shooter/Components/HealthComponent.h"
#include "Shooter/Components/ShootComponent.h"
#include "Shooter/EnemyAIController.h"
#include "Shooter/Interactables/PickupWeapon.h"
#include "Shooter/Player/TPS_PlayerCharacter.h"

static float CurrentDeathImpulse;
static  FName CurrentBoneHit;

//---------------------------------------------------------------------------------------------------------------------------------------
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->bReceivesDecals	  = false;
	bIsDeath					  = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(GetMesh(), "GunSocket");
	
	SpawnFloorBloodArrow = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	SpawnFloorBloodArrow->SetupAttachment(GetMesh());
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddDynamic(this, &ABaseEnemy::OnActorDestroyed);

	ShootComponent = CreateDefaultSubobject<UShootComponent>("Shoot Component");
}

//---------------------------------------------------------------------------------------------------------------------------------------
FName ABaseEnemy::GetHeadBone() const { return HeadBoneName; }

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::OnHit(float DamageTaken,float ShootImpulse, FName& HitBoneName)
{
	HealthComponent->TakeDamage(DamageTaken);
	CurrentDeathImpulse = ShootImpulse;
	CurrentBoneHit = HitBoneName;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::OnActorDestroyed()
{
	bIsDeath = true;
	Controller = nullptr;
	
	HealthComponent->DestroyComponent();

	OnEnemyKilled.Broadcast(this);
	
	DropItem();
	
	DeathVFX();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::InstaKill()
{
	FName name = "head";
	OnHit(HealthComponent->GetMaxHealth(), 100.f, name);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::WarningReceived(ATPS_PlayerCharacter* PlayerRef)
{
	AIController->WarnReceive(PlayerRef);
}

//---------------------------------------------------------------------------------------------------------------------------------------
ATargetPoint* ABaseEnemy::GetCurrentPatrolPoint() const
{
	return PatrolPoints[PatrolIndex];
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	ShootComponent->SetWeaponSkeleton(WeaponMesh, GunShootAnim);
	
	AIController = CastChecked<AEnemyAIController>(GetController());
	BlackBoard = AIController->GetBlackboardComponent();

	GetCharacterMovement()->MaxWalkSpeed = RegularSpeed;
	
	MeleeTimer = MeleeCoolDown;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MeleeTimer += DeltaSeconds;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::ShootStart()
{
	if(bIsDeath) return;

	if(bIsAttacking) return;
	
	PlayAnimMontage(SkeletonShootAnim);
	
	ShootComponent->FireWeapon();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::MeleeAttack()
{
	if(bIsDeath) return;
	
	if(bIsAttacking || MeleeTimer < MeleeCoolDown) return;
	
	bIsAttacking = true;
	MeleeTimer = 0;

	PlayAnimMontage(SkeletonMeleeAnim);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::MeleeAttackStart()
{
	TArray<FHitResult> HitResults;

	const FVector& StartBox = GetActorLocation();
	const FVector& EndBox   = StartBox + (GetActorForwardVector() * MeleeAttackDistance);
	const FVector& BoxSize  = FVector(20.0f, 50.0f, 50.0f);

	TArray<AActor*> IgnoreTheseActors;
	IgnoreTheseActors.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartBox, EndBox, BoxSize,
	GetActorRotation(), ObjectTypes, true, IgnoreTheseActors, EDrawDebugTrace::ForDuration, HitResults, true, 
	FColor::Red, FColor::Green, 3.0f);
	
	for (auto HitResult : HitResults)
	{
		if (ATPS_PlayerCharacter* TempEnemy = Cast<ATPS_PlayerCharacter>(HitResult.GetActor()))
		{
			FName a = "NONE";
			TempEnemy->OnHit(MeleeAttackDamage, 10000.f,a);
			break;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::MeleeAttackEnd()
{
	bIsAttacking = false;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::ChangeToNextWaypoint()
{
	++PatrolIndex;

	if(PatrolIndex > PatrolPoints.Num()-1)
	{
		PatrolIndex = 0;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::PlayerOnSight(bool bAwareOfPlayer)
{
	bIsAwareOfPlayer = bAwareOfPlayer;

	GetCharacterMovement()->MaxWalkSpeed = bIsAwareOfPlayer ? AwareOfPlayerSpeed : RegularSpeed;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::WarnOtherEnemies(ATPS_PlayerCharacter* PlayerRef)
{
	OnPlayerDetected.Broadcast(this, PlayerRef);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::DropItem()
{
	int8 Randomizer = FMath::RandRange(0, PossibleDrops.Num()-1);

	GetWorld()->SpawnActor<APickupWeapon>(PossibleDrops[Randomizer], SpawnFloorBloodArrow->GetComponentLocation(),
											SpawnFloorBloodArrow->GetComponentRotation());
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::DeathVFX() const
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	const auto& PlayerReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	const FVector& DeathImpulseDir = UKismetMathLibrary::Normal(PlayerReference->GetActorForwardVector());

	WeaponMesh->DestroyComponent();
	
	GetMesh()->AddImpulse(DeathImpulseDir * CurrentDeathImpulse, CurrentBoneHit, false);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFX, GetActorLocation(), FRotator::ZeroRotator,
						FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);

	GetWorld()->SpawnActor<ADecalActor>(BloodFloor, SpawnFloorBloodArrow->GetComponentLocation(),
										SpawnFloorBloodArrow->GetComponentRotation(), SpawnParams);
}