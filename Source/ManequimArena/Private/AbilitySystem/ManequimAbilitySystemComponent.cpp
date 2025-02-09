// Copyright to Bue.


#include "AbilitySystem/ManequimAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "ManequimGameplayTags.h"

void UManequimAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
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
/// Binds our EffectApplied function to the OnGameplayEffectAppliedDelegateToSelf.
/// This means that all effects (instant, duration or infinite) that are applied will call this function
/// </summary>
void UManequimAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UManequimAbilitySystemComponent::ClientEffectApplied);

}

/// <summary>
/// Loops through StartUpAbilities and Gives this ability to the Player
/// Also adds the corresponding input Tag for that Ability to be activated via enhanced inputs
/// </summary>
/// <param name="StartUpAbilities"></param>
void UManequimAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbilities)
	{
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(BaseAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

/// <summary>
/// Functions made to activate abilities
/// </summary>
/// <param name="InputTag"></param>
void UManequimAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	//Check if our tag is valid... if not, return
	if (!InputTag.IsValid()) return;
	
	//Loop through our Activable Abilities (Abilities that can be activated... not blocked, etc.)
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//Check if this ability has our pressed tag
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//We can inform the ASC that we are pressing this Ability so we can use it later on the Ability Itself 
			AbilitySpecInputPressed(AbilitySpec);
			//Check if our Ability is Active. We only want to activate the ability if it's not in use already
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

/// <summary>
/// 
/// </summary>
/// <param name="InputTag"></param>
void UManequimAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	//Check if our tag is valid... if not, return
	if (!InputTag.IsValid()) return;

	//Loop through our Activable Abilities (Abilities that can be activated... not blocked, etc.)
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//Check if this ability has our pressed tag
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//On Release, we don't need to activate the Ability, since it has already on Held, 
			//but we want to inform the ASC that that Ability Input was release, so we can decide what to do in the Ability itself.
			AbilitySpecInputReleased(AbilitySpec);

		}
	}
}

