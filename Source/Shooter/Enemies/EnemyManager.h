//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class SHOOTER_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	

public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   //
	//*****************************************************************************//

	//Constructor
	AEnemyManager();
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   //
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   //
	//*****************************************************************************//
	
	
private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							...//
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float DetectingDistance;
	
	TArray<AActor*> AllEnemies;
	TArray<class ABaseEnemy*> AllEnemiesCasted;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...//
	//*****************************************************************************//
	virtual void BeginPlay() override;

	void GetAllEnemiesInScene();

	void BindEnemiesBehaviours();
	
	UFUNCTION()
	void WarnNearEnemies(class ABaseEnemy* Caller, class ATPS_PlayerCharacter* PlayerLocation);
};