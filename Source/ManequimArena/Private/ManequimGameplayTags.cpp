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

}
