//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shooter/Interface/IInteractable.h"
#include "BaseInteractor.generated.h"

UCLASS()
class SHOOTER_API ABaseInteractor : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:

	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor
	ABaseInteractor();

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* StaticMeshComp;
	
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class UBoxComponent* BoxComponent;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//

	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//
	
	virtual void OnInteraction() override;

	void RemoveFromCurrent();
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//

	class ATPS_PlayerCharacter* Player;
	
	UPROPERTY(EditDefaultsOnly, Category = Visuals)
	TSubclassOf<UUserWidget> InteractionUserWidget;
	UUserWidget* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, Category = SFX)
	USoundBase* InteractionSound;
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnInteractionMade() override;

private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//

	void Activate();
	void Deactivate();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult&  SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 
};
