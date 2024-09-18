//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameModeBase.generated.h"

UCLASS()
class SHOOTER_API AShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   //
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   //
	//*****************************************************************************//
	void LevelFinished(bool HasWon);

private:

	//*****************************************************************************//
	//								PRIVATE VARIABLES							...//
	//*****************************************************************************//
	class ATPS_PlayerController* GameController;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	USoundBase* WinSound;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	USoundBase* LoseSound;

	FTimerHandle LevelFinishedTimerHandle;
	FTimerDelegate LevelFinishedTimerDelegate;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...//
	//*****************************************************************************//

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	void LevelSuccess();

	void LevelFailure();
};