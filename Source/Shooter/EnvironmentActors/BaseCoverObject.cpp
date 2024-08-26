//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseCoverObject.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Shooter/Player/TPS_PlayerCharacter.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ABaseCoverObject::ABaseCoverObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->Mobility = EComponentMobility::Static;
	RootComponent = SceneRoot;
	
	Cover_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoverObject"));
	Cover_MeshComponent->SetGenerateOverlapEvents(true);
	Cover_MeshComponent->SetupAttachment(RootComponent);
	Cover_MeshComponent->Mobility = EComponentMobility::Static;
	Cover_MeshComponent->SetCollisionObjectType(ECC_WorldStatic);

	Cover_Enter_BoxComponent = CreateDefaultSubobject<UBoxComponent>("Cover Enter Component");
	Cover_Enter_BoxComponent->SetupAttachment(SceneRoot);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComponent->SetupAttachment(SceneRoot);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::BeginPlay()
{
	Super::BeginPlay();

	Cover_Enter_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCoverObject::OnPlayerBeginOverlapEnterArea);
	Cover_Enter_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseCoverObject::OnPlayerEndOverlapEnterArea);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::OnPlayerBeginOverlapEnterArea(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bIsCoverUsed) return;
	
	if(ATPS_PlayerCharacter* PlayerInside = Cast<ATPS_PlayerCharacter>(OtherActor))
	{
		PlayerInside->SetCoverObject(this);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::OnPlayerEndOverlapEnterArea(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(bIsCoverUsed) return;
	
	if(ATPS_PlayerCharacter* PlayerInside = Cast<ATPS_PlayerCharacter>(OtherActor))
	{
		PlayerInside->SetCoverObject(nullptr);
	}
}