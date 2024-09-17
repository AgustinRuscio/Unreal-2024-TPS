//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameStateBase.generated.h"

UCLASS()
class SHOOTER_API AShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   //
	//*****************************************************************************//

	//Constructor
	AShooterGameStateBase();
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
	virtual void BeginDestroy() override;
	
	void GetAllEnemiesInScene();

	void CheckEnemiesLeft();
	
	void BindEnemiesBehaviours();
	void UnbindEnemiesBehaviours();
	
	UFUNCTION()
	void WarnNearEnemies(class ABaseEnemy* Caller, class ATPS_PlayerCharacter* PlayerLocation);
	
	UFUNCTION()
	void EnemyEliminated(class ABaseEnemy* Caller);
};