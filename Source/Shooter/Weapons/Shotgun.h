//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GameFramework/Actor.h"
#include "Shotgun.generated.h"

UCLASS()
class SHOOTER_API AShotgun : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	AShotgun();

	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
	virtual void FireWeapon() override;

private:
	
	//*****************************************************************************//
	//								Private VARIABLES							   // 
	//*****************************************************************************//
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	int AmountOfShoots;
};