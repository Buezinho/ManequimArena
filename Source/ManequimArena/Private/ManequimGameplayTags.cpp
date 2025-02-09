// Copyright to Bue.


#include "ManequimGameplayTags.h"
#include "GameplayTagsManager.h"

FManequimGameplayTags FManequimGameplayTags::GameplayTags;

/// <summary>
/// Centralizes the initialization of all our gameplay tags and allow an easy way to retrieve them using this singleton
/// </summary>
void FManequimGameplayTags::InitializeNativeGameplayTags()
{
	//Vital Attributes

	//Primary Attributes
	GameplayTags.Attributes_Primary_STR = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.STR"), FString("Increases Physical Damage"));
	GameplayTags.Attributes_Primary_INT = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.INT"), FString("Increases Spell Damage"));
	GameplayTags.Attributes_Primary_RES = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.RES"), FString("Increases Armor and Armor Penetration"));
	GameplayTags.Attributes_Primary_VIG = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.VIG"), FString("Increases Health and Health Regen"));

	//Secondary Attributes
	GameplayTags.Attributes_Secondary_Armor =					UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration =		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Reduces armor from the target"));
	GameplayTags.Attributes_Secondary_BlockChance =				UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("% Chance to reduce damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance =		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("% Chance to deal a critical blow"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage =		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("% of Damage increase in a Critical blow"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduced % of received critical blows"));
	GameplayTags.Attributes_Secondary_HealthRegeneration =		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Health gained by second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration =		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Mana gained by second"));
	GameplayTags.Attributes_Secondary_MaxHealth =				UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Maximum value possible for health"));
	GameplayTags.Attributes_Secondary_MaxMana =					UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Maximum value possible for mana"));


	//Input Actions
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_MB3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.MB3"), FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_Ability1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability1"), FString("Input Tag for Ability 1 Key"));
	GameplayTags.InputTag_Ability2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability2"), FString("Input Tag for Ability 2 Key"));
	GameplayTags.InputTag_Ability3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability3"), FString("Input Tag for Ability 3 Key"));
	GameplayTags.InputTag_Ability4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability4"), FString("Input Tag for Ability 4 Key"));
	GameplayTags.InputTag_AbilityUltimate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ultimate"), FString("Input Tag for Ultimate Key"));
	GameplayTags.InputTag_BaseAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.BaseAttack"), FString("Input Tag for Base Attack Key"));
	GameplayTags.InputTag_StopAction = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.StopAction"), FString("Input Tag for Stop Action Key"));
	GameplayTags.InputTag_CancelAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.CancelAbility"), FString("Input Tag for Cancel Ability Key"));

	//Damage Types
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage delt"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage Type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage Type"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Arcane Damage Type"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage Type"));
	//GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);

	//Resistances
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Resistance to Fire Damage"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning Damage"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Resistance to Arcane Damage"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Resistance to Physical Damage"));

	//Map of Damage types to Resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	//Effects
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Reaction Effect granted after being hit"));
	

}
