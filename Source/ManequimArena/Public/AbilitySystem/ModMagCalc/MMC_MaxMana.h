// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	//MMC's are verry powerful and can capture attributes.
	//For that we need to create this FGameplayEffectAttributeCaptureDefinition variable
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
	
};
