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

	//Resistances
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	//Input GameplayTags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_MB3;
	FGameplayTag InputTag_Ability1;
	FGameplayTag InputTag_Ability2;
	FGameplayTag InputTag_Ability3;
	FGameplayTag InputTag_Ability4;
	FGameplayTag InputTag_AbilityUltimate;
	FGameplayTag InputTag_BaseAttack;
	FGameplayTag InputTag_StopAction;
	FGameplayTag InputTag_CancelAbility;

	//Damage
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	//Map to hold all of all damage types. It will be easier to look for all damage types and it`s resistances
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	//Effects
	FGameplayTag Effects_HitReact;

protected:

private:
	static FManequimGameplayTags GameplayTags;

};
