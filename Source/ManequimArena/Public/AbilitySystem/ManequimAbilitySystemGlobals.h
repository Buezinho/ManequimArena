// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ManequimAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UManequimAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
