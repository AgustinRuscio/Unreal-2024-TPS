//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GameFramework/Actor.h"
#include "Pistol.generated.h"

UCLASS()
class SHOOTER_API APistol : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	APistol();

	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	virtual void FireWeapon() override;
	virtual void FireEnd() override;
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

private:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};