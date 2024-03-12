// Copyright to Bue.


#include "AbilitySystem/ManequimAbilitySystemComponent.h"

void UManequimAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));

	//Get Gameplay Asset Tags
	FGameplayTagContainer TagContainer;
	//Fill our recently created TagContainer with all Asset Tags
	EffectSpec.GetAllAssetTags(TagContainer);
	

	//We can now broadcast our TagContainer to our Widgets if we wish to yse them in our gameplay
	EffectAssetTags.Broadcast(TagContainer);

}

/// <summary>
/// Binds our EffectApplied funtion to the OnGameplayEffectAppliedDelegateToSelf.
/// This means that all effects (instant, duration or infinite) that are applied will call this function
/// </summary>
void UManequimAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UManequimAbilitySystemComponent::EffectApplied);
}

