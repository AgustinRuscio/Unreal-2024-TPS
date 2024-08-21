//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseEnemy.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DecalActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HittedBoneName			   = "NONE";
	DeathImpulse			   = 1000.f;
	GetMesh()->bReceivesDecals = false;

	SpawnFloorBloodArrow = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	SpawnFloorBloodArrow->SetupAttachment(GetMesh());
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddDynamic(this, &ABaseEnemy::OnActorDestroyed);
}

//---------------------------------------------------------------------------------------------------------------------------------------
FName ABaseEnemy::GetHeadBone() { return HeadBoneName; }

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::OnHit(float DamageTaken, FName& HiitedBoneName)
{
	HealthComponent->TakeDamage(DamageTaken);
	HittedBoneName = HiitedBoneName;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::OnActorDestroyed()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	DeathVFX();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseEnemy::DeathVFX()
{
	const auto& PlayerReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	const FVector& DeathImpulseDir = UKismetMathLibrary::Normal(PlayerReference->GetActorForwardVector());
	GetMesh()->AddImpulse(DeathImpulseDir * DeathImpulse, HittedBoneName, false);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFX, GetActorLocation(), FRotator::ZeroRotator,
						FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);

	GetWorld()->SpawnActor<ADecalActor>(BloodFloor, SpawnFloorBloodArrow->GetComponentLocation(),
										SpawnFloorBloodArrow->GetComponentRotation(), SpawnParams);
}
