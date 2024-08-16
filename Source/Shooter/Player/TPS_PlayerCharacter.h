//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "TPS_PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStateAction : uint8
{
	Idle,
	Moving,
	Aiming
};

UCLASS()
class SHOOTER_API ATPS_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ATPS_PlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TSubclassOf<class UCameraShakeBase> CameraShakeHit;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
	void MovePlayer(FVector2d Direction);
	void MovementStart();
	void MovementEnd();
	
	void SprintStart();
	void SprintEnd();
	
	void RotateCamera(FVector2d Direction);
	
	void AimStart();
	void AimEnd();
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//
	
private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							...// 
	//*****************************************************************************//

	bool bCanMove;
	bool bCanMoveCamera;
	bool bIsMoving;
	bool bIsSprinting;

	bool bCanAim;
	bool bIsAiming;
	bool bUnarmed;

	EPlayerStateAction CurrentState;
	
	FTimeline TimeLineSpringArmMoving;
	FTimeline TimeLineSpringArmAiming;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UCurveFloat* FloatCurveSpringArmLength;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	void BindTimeLines();
	void TimeLinesTick(float DeltaSeconds);
	
	UFUNCTION()
	void SetSpringArmMovingSettings(float deltaSeconds) const;
	
	UFUNCTION()
	void SetSpringArmAimingSettings(float deltaSeconds) const;
};