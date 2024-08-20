//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

UINTERFACE(MinimalAPI)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};

class SHOOTER_API IIInteractable
{
	GENERATED_BODY()

public:

	virtual void OnInteraction() = 0;

protected:
	
	virtual void OnInteractionMade() = 0;
};