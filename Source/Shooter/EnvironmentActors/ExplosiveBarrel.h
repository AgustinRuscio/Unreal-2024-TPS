//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shooter/Interface/IDamageable.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class SHOOTER_API AExplosiveBarrel : public AActor, public IIDamageable
{
	GENERATED_BODY()
	
public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	AExplosiveBarrel();

	UPROPERTY(EditAnywhere, Category = Mesh, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
virtual FName GetHeadBone() const override;
	virtual void OnHit(float DamageTaken, float ShootImpulse, FName& BoneHitted) override;
	virtual void OnActorDestroyed() override;
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//
	
private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float ExplosionRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float ExplosionDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* ExplosionParticles;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	void ExplosiveArea();
	void ExplosionVFX();
};