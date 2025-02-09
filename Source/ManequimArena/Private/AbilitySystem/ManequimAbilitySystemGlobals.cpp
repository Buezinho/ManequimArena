// Copyright to Bue.


#include "AbilitySystem/ManequimAbilitySystemGlobals.h"
#include "ManequimAbilityTypes.h"

FGameplayEffectContext* UManequimAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FManequimGameplayEffectContext();
}
