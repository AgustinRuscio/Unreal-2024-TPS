//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shooter/Components/HealthComponent.h"
#include "Shooter/Interface/IDamageable.h"
#include "BaseEnemy.generated.h"

UCLASS()
class SHOOTER_API ABaseEnemy : public ACharacter, public IIDamageable
{
	GENERATED_BODY()
	
public:

	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ABaseEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SpawnFloorBloodArrow;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	virtual FName GetHeadBone() override;
	
	virtual void OnHit(float DamageTaken, FName& HiitedBoneName) override;

	UFUNCTION()
	virtual void OnActorDestroyed() override;
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FName HeadBoneName;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UHealthComponent* HealthComponent;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//
	
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	float DeathImpulse;
	
	FName HittedBoneName;

	UPROPERTY(EditDefaultsOnly, Category = SFX)
	USoundBase* DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BloodVFX;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class ADecalActor> BloodFloor;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	void DeathVFX();
};