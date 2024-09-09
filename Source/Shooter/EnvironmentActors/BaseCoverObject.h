//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "BaseCoverObject.generated.h"

UCLASS()
class SHOOTER_API ABaseCoverObject : public AActor
{
	GENERATED_BODY()
	
public:	
	//*****************************************************************************//
	//						CONSTRUCTOR & PUBLIC COMPONENTS						   // 
	//*****************************************************************************//

	//Constructor;
	ABaseCoverObject();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Cover_MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Cover_One_BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Cover_Two_BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Cover_Three_BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Cover_Four_BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BorderOneArrowComponent;
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BorderTwoArrowComponent;
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BorderThreeArrowComponent;
	UPROPERTY(EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BorderFourArrowComponent;
	
	class UArrowComponent* CurrentCoverArrow;
	
	//*****************************************************************************//
	//								PUBLIC VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PUBLIC METHODS								   // 
	//*****************************************************************************//

	FORCEINLINE bool GetCrouchCover() const { return bIsCrouchedCover; };
	FORCEINLINE FRotator GetCoverRotation() const { return ArrowComponent->GetRelativeRotation(); };

	void SetCoverActor(AActor* CoverActor);
	void LeaveCover();
	
protected:
	
	//*****************************************************************************//
	//								PROTECTED VARIABLES							   // 
	//*****************************************************************************//
	
	//*****************************************************************************//
	//								PROTECTED METHODS							   // 
	//*****************************************************************************//
	
private:
	
	//*****************************************************************************//
	//								PRIVATE VARIABLES							   // 
	//*****************************************************************************//

	bool bIsCoverUsed;

	UPROPERTY(EditAnywhere, Category = Settings)
	bool bIsCrouchedCover;

	AActor* CoveringActor;
	
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class UAnimMontage* CoverAnimMontage;
	
	//*****************************************************************************//
	//								PRIVATE METHODS								...// 
	//*****************************************************************************//
	
	virtual void BeginPlay() override;

	void CalculateCurrentArrow();
	
	UFUNCTION()
	void OnPlayerBeginOverlapEnterArea(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlapEnterArea(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};