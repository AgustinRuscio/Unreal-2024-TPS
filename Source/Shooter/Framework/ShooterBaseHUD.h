//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterBaseHUD.generated.h"

UCLASS()
class SHOOTER_API AShooterBaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   //
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   //
	//*****************************************************************************//
	void UpdateLifeBar(float Value);
	void StealthKill(bool Value);
	
	void DisplayWinningWidget();
	void DisplayLostWidget();

private:
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   //
	//*****************************************************************************//
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<UUserWidget> DeathWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<UUserWidget> WinningWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<class UPlayerHUD> PlayerHUDWidget;
	
	class UUserWidget* DeathHUD;
	class UUserWidget* WinningHUD;
	
	class UUserWidget* CurrentFocusableWidget;

	class UPlayerHUD* PlayerHUD;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								   //
	//*****************************************************************************//
	
	virtual void BeginPlay() override;

	void FocusWidget();
	
	void CreateGameHUD();
	void CreateEndGameHUD();
};
