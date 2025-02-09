// Copyright to Bue.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "ManequimGameplayTags.h"
#include "AbilitySystem/ManequimAttributeSet.h"
#include "AbilitySystem/ManequimAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "ManequimAbilityTypes.h"

struct ManequimDamageStatics
{
	// DECLARE_ATTRIBUTE_CAPTUREDEF creates a FProperty and a FGameplayEffectAttributeCaptureDefinition variables
	// Captures the Attributes so we can use them in our calculations
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	//Maps the Tags to the Capture Defs 
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	ManequimDamageStatics()
	{
		//Defines the Attribute Capture with class, name, source/target and if it should be snapshoted
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, Armor,					Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, ArmorPenetration,		Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, BlockChance,				Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, CriticalHitChance,		Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, CriticalHitDamage,		Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, CriticalHitResistance,	Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, ArcaneResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, FireResistance,			Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, LightningResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UManequimAttributeSet, PhysicalResistance,		Target, false);

		//Map all of our attributes to capture to the corresponding tags
		const FManequimGameplayTags Tags = FManequimGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);

	}
};

static const ManequimDamageStatics DamageStatics()
{
	static ManequimDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//Add our attributes definition to a list that is required for using it during execution
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//------------------------------------------------------------------------------------------------
	// Retrieve usefull information from ExecutionParams
	//------------------------------------------------------------------------------------------------
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//If our ASC is not null, populate the Avatar
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	//FGameplayEffectContext* Context = EffectContextHandle.Get();
	//FManequimGameplayEffectContext* ManequimContext = static_cast<FManequimGameplayEffectContext*>(Context);
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	// Capture Attributes
	//------------------------------------------------------------------------------------------------
	/* WE NEED TO CLAMP PROPERTIES HERE... REMEMBER THAT PRE ATTRIBUTE CHANGE IS NOT CALLED ON EXECUTION CALCULATION*/

	//Variable that we will have to use in order to capture the attributes we are interested on
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// Get Damage Set by Caller	for each Damage Type
	float Damage = 0.f;//Spec.GetSetByCallerMagnitude(FManequimGameplayTags::Get().Damage);
	for (const auto& Pair : FManequimGameplayTags::Get().DamageTypesToResistances)
	{
		//Create Variables to facilitate the understanding and allow us to map better
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		//Check if we have the said resistance in our map, otherwise it will crash. Warn us on the failed check
		checkf(ManequimDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: %s in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = ManequimDamageStatics().TagsToCaptureDefs[ResistanceTag];

		//Gets the Damage Magnitude on the Gameplay Effect associated with this Damage Type Tag
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		//Captures the magnitude of our attribute in a variable 
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		//Reduces the damage delt by x% of the corresponding resistance
		DamageTypeValue *= (100.f - Resistance) / 100.f;
		
		//If the Tag is not found, the default value is zero, so it's safe to add all damage types to our Damage
		Damage += DamageTypeValue;
	}

	//Get Target Armor Attribute Magniture and sets to a variable
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	//Get Source Armor Penetration Attribute Magniture and sets to a variable
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	//Block chance on target. If Block, damage is halved
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	//Critical Hit Chance Attribute Magniture and sets to a variable
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	//Critical Hit Chance Attribute Magniture and sets to a variable
	float SourceCriticalHitDamage = 1.5f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	//Critical Hit Chance Attribute Magniture and sets to a variable
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	
	
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	// Exec Calculation
	//------------------------------------------------------------------------------------------------
	
	//Get Damage coefficients from our Character Class Info
	float ArmorPenetrationCoefficient = 1.f;
	float EffectiveArmorCoefficient = 1.f;
	float EffectiveCriticalHitResistanceCoefficient = 1.f;

	const UCharacterClassInfo* CharacterClassInfo = UManequimAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const FRealCurve* EffectiveCriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveCriticalHitResistance"), FString());

	ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	EffectiveCriticalHitResistanceCoefficient = EffectiveCriticalHitResistanceCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	bool bCritical = FMath::RandRange(1, 100) < ((SourceCriticalHitChance * 100) - TargetCriticalHitResistance * EffectiveCriticalHitResistanceCoefficient);
	UManequimAbilitySystemLibrary::SetCriticalHit(EffectContextHandle, bCritical);

	Damage = bCritical ? Damage * SourceCriticalHitDamage : Damage;


	bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	if (!bCritical)
	{
		UManequimAbilitySystemLibrary::SetBlockedHit(EffectContextHandle, bBlocked);
		// if blocked, damage is halved
		Damage = (bBlocked) ? Damage / 2.f : Damage;
	}	

	float EffectiveArmor = ((100 -  TargetArmor) / 100) + ( SourceArmorPenetration * ArmorPenetrationCoefficient / 100);

	Damage = Damage * EffectiveArmor * EffectiveArmorCoefficient;

		
	//------------------------------------------------------------------------------------------------


	/* DEFINES THE OUTPUT ATTRIBUTE MODIFIER TO BE USED IN OUR GAMEPLAY EFFECT */
	//We are NOT LIMITED to just 1 Output Eveluated Modifier. We cann add several
	//const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	const FGameplayModifierEvaluatedData EvaluatedData(UManequimAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);


	
}
