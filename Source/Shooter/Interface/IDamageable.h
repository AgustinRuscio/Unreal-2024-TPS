//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

UINTERFACE(MinimalAPI)
class UIDamageable : public UInterface
{
	GENERATED_BODY()
};

class SHOOTER_API IIDamageable
{
	GENERATED_BODY()

public:
	virtual FName GetHeadBone() const = 0;
	
	virtual void OnHit(float DamageTaken,float ShootImpulse,FName& BoneHit) = 0;
	virtual void OnActorDestroyed() = 0;
};