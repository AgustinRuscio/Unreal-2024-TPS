//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "EnemyManager.h"

#include "BaseEnemy.h"
#include "VectorTypes.h"
#include "Kismet/GameplayStatics.h"

//---------------------------------------------------------------------------------------------------------------------------------------
AEnemyManager::AEnemyManager()
{
 	PrimaryActorTick.bCanEverTick = true;
}

