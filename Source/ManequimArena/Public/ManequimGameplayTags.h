// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 * Singleton containing native Gameplay Tags
 */

/// <summary>
/// Singleton containing native Gameplay Tags
/// </summary>
struct FManequimGameplayTags
{
public:
	static const FManequimGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();


	//Vital Attributes
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;	
	
	//Primary Attributes
	FGameplayTag Attributes_Primary_STR;
	FGameplayTag Attributes_Primary_INT;
	FGameplayTag Attributes_Primary_RES;
	FGameplayTag Attributes_Primary_VIG;

	//Secondary Attributes
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

protected:

private:
	static FManequimGameplayTags GameplayTags;

};
