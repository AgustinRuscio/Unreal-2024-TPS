//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shooter/EnumContainer.h"
#include "WeaponHUD.generated.h"


UCLASS()
class SHOOTER_API UWeaponHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	UFUNCTION(BlueprintImplementableEvent)
	void SetWeaponType(EWeaponType OwnerType);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmo(class ABaseWeapon* OwnerWeapon);

protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Visuals)
	UTexture* PistolImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Visuals)
	UTexture* RifleImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Visuals)
	UTexture* ShotgunImage;

	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//

private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							...// 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//
};