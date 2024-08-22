//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairHUD.generated.h"


UCLASS()
class SHOOTER_API UCrosshairHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeEquippedWeapon(EWeaponType NewWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponUnlocked(EWeaponType NewWeapon);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimStart();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimEnd();
};