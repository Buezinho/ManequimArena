// Copyright to Bue.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/ManequimAttributeSet.h"
#include "Interaction/CombatInterface.h"

/// <summary>
/// In our contructor we set all variables required in the Blueprint to specify a Modifier.
/// Attribure to Capture, Apply to source or target and Snapshot
/// </summary>
UMMC_MaxHealth::UMMC_MaxHealth()
{
	//Capture our atribute in our variable
	VigorDef.AttributeToCapture = UManequimAttributeSet::GetVIGAttribute();
	//Specify from whom we are capturing our attribute from (source or target)
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//Specify if the modifier needs to be snapshooted.
	VigorDef.bSnapshot = false;

	//When the modifier is applied, we need to make sure that (in this case) the VIG from the Target is captured!!!
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//Variable that we will have to use in order to capture the attributes we are interested on
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	// After calling this function, Vigor will now have the Vigor Attribute Magnitude value of the VIG Attribute from the target
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);

	//Normally we clamp it
	Vigor = FMath::Max<float>(Vigor, 10.f);

	//We can find the UObject that is casting this effect in the Spec context
	//We can cast it to our Interface to check it's level
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 PlayerLevel = 0.f;
	if(CombatInterface != nullptr)
	{ 
		PlayerLevel = CombatInterface->GetPlayerLevel();
	}

	//Now we can run our calculations to return the ModMagnitudeCalculation (MMC)
	return 45.f + (2.5f * Vigor) + (10.f * PlayerLevel);
}
