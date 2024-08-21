//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "HealthComponent.h"

//---------------------------------------------------------------------------------------------------------------------------------------
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UHealthComponent::TakeDamage(float Damage)
{
	CurrentHealth -= Damage;

	if(CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		OnDeath.Broadcast();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void UHealthComponent::Heal(float HealAmount)
{
	CurrentHealth += HealAmount;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}