//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class SHOOTER_API ARifle : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ARifle();

	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
	virtual void FireWeapon (FVector StartShootPoint, FVector ForwardShootPoint);
};
