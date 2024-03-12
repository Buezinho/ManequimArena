// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ManequimAbilitySystemComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam()
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UManequimAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

public:
	void AbilityActorInfoSet();
	FEffectAssetTags EffectAssetTags;
	
};
