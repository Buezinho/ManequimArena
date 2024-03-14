// Copyright to Bue.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/ManequimAttributeSet.h"
#include "Interaction/CombatInterface.h"

/// <summary>
/// In our contructor we set all variables required in the Blueprint to specify a Modifier.
/// Attribure to Capture, Apply to source or target and Snapshot
/// </summary>
UMMC_MaxMana::UMMC_MaxMana()
{
	//Capture our atribute in our variable
	IntelligenceDef.AttributeToCapture = UManequimAttributeSet::GetINTAttribute();
	//Specify from whom we are capturing our attribute from (source or target)
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//Specify if the modifier needs to be snapshooted.
	IntelligenceDef.bSnapshot = false;

	//When the modifier is applied, we need to make sure that (in this case) the VIG from the Target is captured!!!
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//Variable that we will have to use in order to capture the attributes we are interested on
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	// After calling this function, Vigor will now have the Vigor Attribute Magnitude value of the VIG Attribute from the target
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);

	//Normally we clamp it
	Intelligence = FMath::Max<float>(Intelligence, 10.f);

	//We can find the UObject that is casting this effect in the Spec context
	//We can cast it to our Interface to check it's level
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 PlayerLevel = 0.f;
	if (CombatInterface != nullptr)
	{
		PlayerLevel = CombatInterface->GetPlayerLevel();
	}

	//Now we can run our calculations to return the ModMagnitudeCalculation (MMC)
	return 25.f + (2.0f * Intelligence) + (15.f * PlayerLevel);
}
