//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractor.h"
#include "GameFramework/Actor.h"
#include "Shooter/EnumContainer.h"
#include "PickupPistol.generated.h"


UCLASS()
class SHOOTER_API APickupPistol : public ABaseInteractor
{
	GENERATED_BODY()
	
public:

	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	APickupPistol();
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	virtual void OnInteraction() override;
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	EWeaponType ThisWeaponType;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	TSubclassOf<class UGunPickupHUD> PickupWidget;
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