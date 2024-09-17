//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllEnemyEliminated);

UCLASS()
class SHOOTER_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	

public:
	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   //
	//*****************************************************************************//

	//Constructor
	AEnemyManager();
};