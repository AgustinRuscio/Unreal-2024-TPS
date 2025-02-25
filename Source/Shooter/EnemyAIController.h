//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


UCLASS()
class SHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   //
	//*****************************************************************************//

	//Constructor
	AEnemyAIController();

	UPROPERTY(EditDefaultsOnly, Category = Components, meta=(AllowPrivateAccess = "true"))
	UAIPerceptionComponent* IaPerceptionComponent;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   //
	//*****************************************************************************//

	UFUNCTION(BlueprintImplementableEvent)
	void WarnReceive(class ATPS_PlayerCharacter* PlayerRef);
	
private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							   //
	//*****************************************************************************//
	
	UPROPERTY(EditDefaultsOnly, Category = AI)
	UBehaviorTree* BehaviourTree;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								   //
	//*****************************************************************************//

	virtual void BeginPlay() override;
};