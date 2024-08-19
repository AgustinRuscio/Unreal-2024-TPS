//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "BaseInteractor.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/Player/TPS_PlayerCharacter.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ABaseInteractor::ABaseInteractor()
{
 	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	BoxComponent->SetupAttachment(StaticMeshComp);
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractor::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseInteractor::OnOverlapEnd);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::OnInteraction()
{
	OnInteractionMade();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::RemoveFromCurrent()
{
	Deactivate();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionWidget = CreateWidget(GetWorld(), InteractionUserWidget);

	Player = CastChecked<ATPS_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::OnInteractionMade()
{
	Destroy();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::Activate()
{
	InteractionWidget->AddToViewport(2);
	Player->SetInteractable(this);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::Deactivate()
{
	InteractionWidget->RemoveFromParent();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Player == Cast<ATPS_PlayerCharacter>(OtherActor)) 
	{
		Activate();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ABaseInteractor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Player == Cast<ATPS_PlayerCharacter>(OtherActor)) 
	{
		Deactivate();
	}
}