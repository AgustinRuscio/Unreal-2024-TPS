//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//


#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"

//---------------------------------------------------------------------------------------------------------------------------------------
AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	IaPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);
}