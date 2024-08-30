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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetWillPatrol() const { return bWillPatrol; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsAwareOfPlayerPresence() const { return bIsAwareOfPlayer; };
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetShootDistance() const { return ShootDistance; };
	
	UFUNCTION(BlueprintCallable)
	class ATargetPoint* GetCurrentPatrolPoint() const;
	
	virtual FName GetHeadBone() const override;
	
	virtual void OnHit(float DamageTaken,float ShootImpulse, FName& HiitedBoneName) override;

	UFUNCTION()
	virtual void OnActorDestroyed() override;

	UFUNCTION(BlueprintCallable)
	void PlayerOnSight(bool bAwareOfPlayer);
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float RegularSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float AwareOfPlayerSpeed;
	
	bool bIsAwareOfPlayer;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FName HeadBoneName;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UHealthComponent* HealthComponent;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ShootStart();

	UFUNCTION(BlueprintCallable)
	void ChangeToNextWaypoint();
	
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditAnywhere, Category = Settings)
	bool bWillPatrol;

	int PatrolIndex = 0;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float ShootDistance;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float ShootDamage;
	
	float DeathImpulse;
	
	FName HittedBoneName;

	UPROPERTY(EditAnywhere, Category = Patrol)
	TArray<class ATargetPoint*> PatrolPoints;
	
	UPROPERTY(EditDefaultsOnly, Category = SFX)
	USoundBase* DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BloodVFX;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class ADecalActor> BloodFloor;

	class UBlackboardComponent* BlackBoard;
	
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	void DeathVFX();
};