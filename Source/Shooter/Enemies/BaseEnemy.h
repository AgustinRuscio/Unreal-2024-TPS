//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shooter/EnumContainer.h"
#include "Shooter/Interface/IDamageable.h"
#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerDetected, ABaseEnemy*, DetectorEnemy, class ATPS_PlayerCharacter*, PlayerRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyEliminated,  ABaseEnemy*, DeathEnemy);

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
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SpawnFloorBloodArrow;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   //
	//*****************************************************************************//

	FPlayerDetected OnPlayerDetected;
	FEnemyEliminated OnEnemyKilled;
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   //
	//*****************************************************************************//
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsAwareOfPlayerPresence() const { return bIsAwareOfPlayer; };
	
	virtual FName GetHeadBone() const override;
	
	virtual void OnHit(float DamageTaken,float ShootImpulse, FName& HitBoneName) override;

	UFUNCTION()
	virtual void OnActorDestroyed() override;

	void InstaKill();
	
	void WarningReceived(class ATPS_PlayerCharacter* PlayerRef);
	
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
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
	class UShootComponent* ShootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	EWeaponType EnemyWeaponType;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = VFX)
	UAnimSequence* GunShootAnim;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = VFX)
	UAnimMontage* SkeletonShootAnim;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = VFX)
	UAnimMontage* SkeletonMeleeAnim;

	class AEnemyAIController* AIController;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   //
	//*****************************************************************************//
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsDeath() const { return bIsDeath; };
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetWillPatrol() const { return bWillPatrol; };
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetShootDistance() const { return ShootDistance; };
	
	UFUNCTION(BlueprintCallable)
	class ATargetPoint* GetCurrentPatrolPoint() const;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void ShootStart();

	UFUNCTION(BlueprintCallable)
	void MeleeAttack();
	
	UFUNCTION(BlueprintCallable)
	void MeleeAttackStart();
	
	UFUNCTION(BlueprintCallable)
	void MeleeAttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void ChangeToNextWaypoint();
	
	UFUNCTION(BlueprintCallable)
	void PlayerOnSight(bool bAwareOfPlayer);
	
	UFUNCTION(BlueprintCallable)
	void WarnOtherEnemies(class ATPS_PlayerCharacter* PlayerRef);
	
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   //
	//*****************************************************************************//
	bool bIsAttacking;
	bool bIsDeath;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	bool bWillPatrol;

	int PatrolIndex = 0;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float ShootDistance;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float ShootDamage;

	UPROPERTY(EditAnywhere, Category = Settings)
	float MeleeAttackDistance;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float MeleeAttackDamage;
	
	float MeleeTimer;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float MeleeCoolDown;
	
	UPROPERTY(EditAnywhere, Category = Patrol)
	TArray<class ATargetPoint*> PatrolPoints;
	
	UPROPERTY(EditDefaultsOnly, Category = SFX)
	USoundBase* DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UParticleSystem* BloodVFX;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class ADecalActor> BloodFloor;

	class UBlackboardComponent* BlackBoard;
	
	UPROPERTY(EditDefaultsOnly, Category = Drop)
	TArray<TSubclassOf<class APickupWeapon>> PossibleDrops;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...//
	//*****************************************************************************//

	void DropItem();
	void DeathVFX() const;
};