//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "ExplosiveBarrel.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//---------------------------------------------------------------------------------------------------------------------------------------
AExplosiveBarrel::AExplosiveBarrel()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
}

//---------------------------------------------------------------------------------------------------------------------------------------
FName AExplosiveBarrel::GetHeadBone() { return "None"; }

//---------------------------------------------------------------------------------------------------------------------------------------
void AExplosiveBarrel::OnHit(float DamageTaken, FName& BoneHitted)
{
	ExplosiveArea();

	ExplosionVFX();

	Destroy();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AExplosiveBarrel::OnActorDestroyed() { }

//---------------------------------------------------------------------------------------------------------------------------------------
void AExplosiveBarrel::ExplosiveArea()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(static_cast<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	UClass* seekClass = AActor::StaticClass(); 
	TArray<AActor*> OutActors;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplosionRadius, ObjectTypes, seekClass, ActorsToIgnore, OutActors);
	
	for (auto ObjectType : OutActors)
	{
		if(IIDamageable* Comp = Cast<IIDamageable>(ObjectType))
		{
			FName j = "NONE";
			Comp->OnHit(ExplosionDamage, j);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AExplosiveBarrel::ExplosionVFX()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), GetActorRotation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation(), FRotator::ZeroRotator,
							FVector(7.5f), true, EPSCPoolMethod::AutoRelease, true);
}