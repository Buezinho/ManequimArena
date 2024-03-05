// Copyright to Bue.


#include "AbilitySystem/ManequimAttributeSet.h"
#include "Net/UnrealNetwork.h"

UManequimAttributeSet::UManequimAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(120.f);
	InitMana(50.f);
	InitMaxMana(80.f);

}

void UManequimAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//In here we register variables for replication
	//COND_None = Replicate always / REPNOTIFY_Aways = always notify changes
	//For our simple game this will sufice
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UManequimAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Health, OldHealth);
}
void UManequimAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, MaxHealth, OldMaxHealth);
}
void UManequimAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Mana, OldMana);
}
void UManequimAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, MaxMana, OldMaxMana);
}
