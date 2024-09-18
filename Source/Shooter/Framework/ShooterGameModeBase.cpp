//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "ShooterGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Shooter/Player/TPS_PlayerController.h"

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameModeBase::LevelFinished(bool HasWon)
{
	LevelFinishedTimerDelegate.BindLambda([=]
	{
		HasWon ? LevelSuccess() : LevelFailure();
	});

	if(!GetWorld()->GetTimerManager().IsTimerActive(LevelFinishedTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(LevelFinishedTimerHandle, LevelFinishedTimerDelegate, 2.f, false);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameController = Cast<ATPS_PlayerController>(GetWorld()->GetFirstPlayerController());
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameModeBase::BeginDestroy()
{
	Super::BeginDestroy();

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LevelFinishedTimerHandle);
	}

	if(LevelFinishedTimerDelegate.IsBound())
	{
		LevelFinishedTimerDelegate.Unbind();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameModeBase::LevelSuccess()
{
	GameController->OnLevelWin();

	UGameplayStatics::PlaySound2D(GetWorld(), WinSound);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameModeBase::LevelFailure()
{
	GameController->OnLevelLose();

	UGameplayStatics::PlaySound2D(GetWorld(), LoseSound);
}