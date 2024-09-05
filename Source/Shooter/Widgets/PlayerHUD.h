//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS()
class SHOOTER_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnLifeBarUpdate(float LifeValue);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void StealthKillWidget(bool IsActive);
};