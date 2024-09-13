//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseCoverObject.h"

#include "VectorTypes.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shooter/Player/TPS_PlayerCharacter.h"

class ATPS_PlayerCharacter;
//---------------------------------------------------------------------------------------------------------------------------------------
ABaseCoverObject::ABaseCoverObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->Mobility = EComponentMobility::Static;
	RootComponent = SceneRoot;
	
	//Mesh
	Cover_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoverObject"));
	Cover_MeshComponent->SetGenerateOverlapEvents(true);
	Cover_MeshComponent->SetupAttachment(RootComponent);
	Cover_MeshComponent->Mobility = EComponentMobility::Static;
	Cover_MeshComponent->SetCollisionObjectType(ECC_WorldStatic);

	//Box Components
	Cover_One_BoxComponent = CreateDefaultSubobject<UBoxComponent>("Cover One Component");
	Cover_One_BoxComponent->SetupAttachment(SceneRoot);
	
	Cover_Two_BoxComponent = CreateDefaultSubobject<UBoxComponent>("Cover Two Component");
	Cover_Two_BoxComponent->SetupAttachment(SceneRoot);
	
	Cover_Three_BoxComponent = CreateDefaultSubobject<UBoxComponent>("Cover Three Component");
	Cover_Three_BoxComponent->SetupAttachment(SceneRoot);
	
	Cover_Four_BoxComponent = CreateDefaultSubobject<UBoxComponent>("Cover Four Component");
	Cover_Four_BoxComponent->SetupAttachment(SceneRoot);

	//Arrows
	BorderOneArrowComponent = CreateDefaultSubobject<UArrowComponent>("Border Arrow One");
	BorderOneArrowComponent->SetupAttachment(SceneRoot);
	
	BorderTwoArrowComponent = CreateDefaultSubobject<UArrowComponent>("Border Arrow Two");
	BorderTwoArrowComponent->SetupAttachment(SceneRoot);
	
	BorderThreeArrowComponent = CreateDefaultSubobject<UArrowComponent>("Border Arrow Three");
	BorderThreeArrowComponent->SetupAttachment(SceneRoot);
	
	BorderFourArrowComponent = CreateDefaultSubobject<UArrowComponent>("Border Arrow Four");
	BorderFourArrowComponent->SetupAttachment(SceneRoot);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::SetCoverActor(AActor* CoverActor)
{
	bIsCoverUsed  = true;
	CoveringActor = CoverActor;
	
	CalculateCurrentArrow();
	
	CoveringActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CoveringActor->SetActorLocation(CurrentCoverArrow->GetComponentLocation());
	CoveringActor->SetActorRotation(CurrentCoverArrow->GetComponentRotation());

	Cover_One_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Cover_Two_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Cover_Three_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Cover_Four_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::LeaveCover()
{
	CoveringActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CoveringActor->SetActorLocation(CurrentCoverArrow->GetComponentLocation());
	CoveringActor->SetActorRotation(CurrentCoverArrow->GetComponentRotation());
	
	bIsCoverUsed  = false;
	CoveringActor = nullptr;

	Cover_One_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Cover_Two_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Cover_Three_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Cover_Four_BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::BeginPlay()
{
	Super::BeginPlay();

	Cover_One_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCoverObject::OnPlayerBeginOverlapEnterArea);
	Cover_Two_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCoverObject::OnPlayerBeginOverlapEnterArea);
	Cover_Three_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCoverObject::OnPlayerBeginOverlapEnterArea);
	Cover_Four_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCoverObject::OnPlayerBeginOverlapEnterArea);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::CalculateCurrentArrow()
{
	float LowestDistance = 900000;

	TArray<UArrowComponent*> ArrowsAUX;
	ArrowsAUX.Add(BorderOneArrowComponent);
	ArrowsAUX.Add(BorderTwoArrowComponent);
	ArrowsAUX.Add(BorderThreeArrowComponent);
	ArrowsAUX.Add(BorderFourArrowComponent);

	for (auto Element : ArrowsAUX)
	{
		auto CurrentDistance = UE::Geometry::Distance(CoveringActor->GetActorLocation(), Element->GetComponentLocation());
		
		if(CurrentDistance < LowestDistance)
		{
			CurrentCoverArrow = Element;
			LowestDistance	  = CurrentDistance;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::OnPlayerBeginOverlapEnterArea(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bIsCoverUsed) return;
	
	if(ATPS_PlayerCharacter* PlayerInside = Cast<ATPS_PlayerCharacter>(OtherActor))
	{
		PlayerInside->GetCharacterMovement()->StopMovementImmediately();
		UE_LOG(LogTemp, Warning, TEXT("UEpa"));
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseCoverObject::OnPlayerEndOverlapEnterArea(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(bIsCoverUsed) return;
	
	if(ATPS_PlayerCharacter* PlayerInside = Cast<ATPS_PlayerCharacter>(OtherActor))
	{

	}
}