//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractor.h"
#include "GameFramework/Actor.h"
#include "Shooter/EnumContainer.h"
#include "PickupWeapon.generated.h"


UCLASS()
class SHOOTER_API APickupWeapon : public ABaseInteractor
{
	GENERATED_BODY()
	
public:

	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	APickupWeapon();
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	virtual void OnInteraction() override;
	virtual void Activate() override;
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	EWeaponType ThisWeaponType;
	
	class UGunPickupHUD* PickupHUD;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//

	virtual void BeginPlay() override;
	
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//
};