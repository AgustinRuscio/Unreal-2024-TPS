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
	DetectingDistance = 1000.f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetAllEnemiesInScene();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AEnemyManager::GetAllEnemiesInScene()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), AllEnemies);

	for (auto Element : AllEnemies)
	{
		if(auto current = Cast<ABaseEnemy>(Element))
		{
			AllEnemiesCasted.Add(current);
		}
	}

	BindEnemiesBehaviours();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AEnemyManager::BindEnemiesBehaviours()
{
	for (auto Element : AllEnemiesCasted)
	{
		Element->OnPlayerDetected.AddDynamic(this, &AEnemyManager::WarnNearEnemies);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AEnemyManager::WarnNearEnemies(ABaseEnemy* Caller, ATPS_PlayerCharacter* PlayerLocation)
{
	for (auto Element : AllEnemiesCasted)
	{
		if(Element == Caller) continue;
		if(UE::Geometry::Distance(Element->GetActorLocation(), Caller->GetActorLocation()) > DetectingDistance) continue;

		Element->WarningReceived(PlayerLocation);
	}
}