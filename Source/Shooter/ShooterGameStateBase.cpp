//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "ShooterGameStateBase.h"

#include "ShooterGameModeBase.h"
#include "VectorTypes.h"
#include "Enemies/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"


//---------------------------------------------------------------------------------------------------------------------------------------
AShooterGameStateBase::AShooterGameStateBase()
{
 	PrimaryActorTick.bCanEverTick = true;
	DetectingDistance = 1000.f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetAllEnemiesInScene();

	UE_LOG(LogTemp, Warning, TEXT("AShooterGameStateBase::BeginPlay"));
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::BeginDestroy()
{
	Super::BeginDestroy();
	
	UnbindEnemiesBehaviours();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::GetAllEnemiesInScene()
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
void AShooterGameStateBase::CheckEnemiesLeft()
{
	if (AllEnemiesCasted.Num() <= 0)
	{
		AShooterGameModeBase* thisGameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
		thisGameMode->LevelFinished(true);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::BindEnemiesBehaviours()
{
	for (auto Element : AllEnemiesCasted)
	{
		Element->OnPlayerDetected.AddDynamic(this, &AShooterGameStateBase::WarnNearEnemies);
		Element->OnEnemyKilled.AddDynamic(this, &AShooterGameStateBase::EnemyEliminated);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::UnbindEnemiesBehaviours()
{
	for (auto Element : AllEnemiesCasted)
	{
		Element->OnPlayerDetected.RemoveDynamic(this, &AShooterGameStateBase::WarnNearEnemies);
		Element->OnEnemyKilled.RemoveDynamic(this, &AShooterGameStateBase::EnemyEliminated);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::WarnNearEnemies(ABaseEnemy* Caller, ATPS_PlayerCharacter* PlayerLocation)
{
	for (auto Element : AllEnemiesCasted)
	{
		if(Element == Caller) continue;
		if(UE::Geometry::Distance(Element->GetActorLocation(), Caller->GetActorLocation()) > DetectingDistance) continue;

		Element->WarningReceived(PlayerLocation);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterGameStateBase::EnemyEliminated(class ABaseEnemy* Caller)
{
	if(AllEnemiesCasted.Contains(Caller))
		AllEnemiesCasted.Remove(Caller);

	CheckEnemiesLeft();
}