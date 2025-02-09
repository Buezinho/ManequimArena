// Copyright to Bue.


#include "AbilitySystem/ManequimAttributeSet.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ManequimGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer/MainPlayerController.h"
#include "AbilitySystem/ManequimAbilitySystemLibrary.h"

UManequimAttributeSet::UManequimAttributeSet()
{
	/*InitHealth(50.f);
	InitMana(50.f);*/

	//Store our gameplaytags in a local constant
	const FManequimGameplayTags& GameplayTags = FManequimGameplayTags::Get();

	//Static Bind Delegates are powerfull tools that allows us create a function pointer with specific signature (in our case 0 params returning a FGameplayAttribute)
	//and assing any kind of Function that matches that signature. This allows us to call it using our newly created pointer as example bellow
	//FunctionPointer = GetINTAttribute;
	//FGameplayAttribute a = FunctionPointer();

	// Old --------------------------------------------------------------------
	//Creates a delegate for our STR attribute and binds it to GetSTRAttribute
	//FAttributeSignature STRDelegate;
	//STRDelegate.BindStatic(this->GetSTRAttribute);
	// TagsToAttributes.Add(GameplayTags.Attributes_Primary_STR, STRDelegate);
	// New --------------------------------------------------------------------
	// 
	//Because we changed our Map from FAttributeSignature to FFuncPtr the can pass our Attribute functions directly

	//Primary Attributes 
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_STR, this->GetSTRAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_INT, this->GetINTAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_RES, this->GetRESAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_VIG, this->GetVIGAttribute);

	//Secondary Attributes 
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, this->GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, this->GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, this->GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, this->GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, this->GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, this->GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, this->GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, this->GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, this->GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, this->GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, this->GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, this->GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, this->GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, this->GetPhysicalResistanceAttribute);
	
}

void UManequimAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//In here we register variables for replication
	//COND_None = Replicate always / REPNOTIFY_Aways = always notify changes
	//For our simple game this will sufice

	/*
	 *  Primary Attributes Lifetime Notify Implementation
	 */

	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, STR, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, INT, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, RES, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, VIG, COND_None, REPNOTIFY_Always);

	/*
	 *  Secondary Attributes Lifetime Notify Implementation
	 */

	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	/*
	 *  Vital Attributes Lifetime Notify Implementation
	 */

	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Health, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
}

void UManequimAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
		//UE_LOG(LogTemp, Warning, TEXT("Changet Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), NewValue);
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
		/*UE_LOG(LogTemp, Warning, TEXT("Mana: %f"), NewValue);*/
	}
}

/// <summary>
/// Function to browse through our FGameplayEffectModCallbackData& Data and fill the FEffectProperties Struct with all handfull variables that we will need to use 
/// </summary>
/// <param name="Data"></param>
/// <param name="Properties"></param>
void UManequimAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const
{
	Properties.EffectContextHandle = Data.EffectSpec.GetContext();
	Properties.SourceASC = Properties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Properties.SourceASC) && Properties.SourceASC->AbilityActorInfo.IsValid() && Properties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.SourceAvatarActor = Properties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Properties.SourceController = Properties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Properties.SourceController == nullptr && Properties.SourceAvatarActor != nullptr)
		{
			//If we can't get the Player Controller from the ASC, we can try getting it from the Pawn itself
			if (const APawn* Pawn = Cast<APawn>(Properties.SourceAvatarActor))
			{
				Properties.SourceController = Pawn->GetController();
			}
		}
		if (Properties.SourceController != nullptr)
		{
			Properties.SourceCharacter = Cast<ACharacter>(Properties.SourceController->GetPawn());
		}

	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		//UAbilitySystemComponent* TargetASC = Data.Target;
		Properties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetAvatarActor);
		Properties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.TargetAvatarActor);
	}
}

void UManequimAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//We will need several information from Data that we will use, so it's a good pratcice to store it on variables

	//Source = causer of the effect, Target = Target of the effect (owner of this AttributeSet)

	FEffectProperties Properties;

	SetEffectProperties(Data, Properties);
	

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changet Health on %s, Health: %f"), *Properties.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	/*
	* --------------------------------------------------------------------------------------------
	* META ATTRIBUTES POST GAMEPLAY EFFECT HANDLING
	* --------------------------------------------------------------------------------------------
	*/
	//We handle Meta Attributes on the PostGameplayEffect
	//For Incoming Damage, if it was modified, we need to apply the effect to our health and zero out the Incoming Damage
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalIncomingDamage > 0)
		{
			//So far, IncomingDamage will be applied directly to our health (just as before), but now using the Meta Attribute staging process
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			//If our Health is equals or less than 0, we can set a local varialbe to use later
			const bool bFatal = NewHealth <= 0.f;

			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Properties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}
			else
			{
				//TODO: Handle MICROSTUNS in a better way later
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FManequimGameplayTags::Get().Effects_HitReact);
				Properties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			

			bool bBlockedHit = UManequimAbilitySystemLibrary::IsBlockedHit(Properties.EffectContextHandle);
			bool bCriticalHit = UManequimAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle);
			ShowFloatingText(Properties, LocalIncomingDamage, bBlockedHit, bCriticalHit);
		}
	}

	//--------------------------------------------------------------------------------------------
}

/// <summary>
/// Show Floating Text will spawn ou Floating Text Widget, attach it to the Taget, Detach it and then fade
/// </summary>
/// <param name="Properties"></param>
/// <param name="Damage"></param>
void UManequimAttributeSet::ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bBlockedHit, bool bCriticalHit)
{
	//Either if the damage was fatal orr not, we have to spawn the Floating Text for the Damage
			// We only won't show self damage******* TODO: Maybe later we can show anyway
			//TODO: implement colors on the damage text
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		//Because ShowFloatingText is being called ONLY when we have the META ATTRIBUTE IncomingDamage, when calling UGameplayStatics we are always returning the server PlayerController
		// AMainPlayerController* PC = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(Properties.SourceCharacter, 0));
		//Instead we can get the correct controller directly from the SourceCharacter in Properties

		AMainPlayerController* PC = Cast<AMainPlayerController>(Properties.SourceCharacter->Controller);
		if (PC)
		{
			//Show Damage Number will spawn ou Floating Text Widget, attach it to the Taget, Detach it and then fade
			PC->ShowDamageNumber(Damage, Properties.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

/*
 *  Primary Attributes Rep Notify Implementation
 */

void UManequimAttributeSet::OnRep_STR(const FGameplayAttributeData OldSTR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, STR, OldSTR);
}

void UManequimAttributeSet::OnRep_INT(const FGameplayAttributeData OldINT) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, INT, OldINT);
}

void UManequimAttributeSet::OnRep_RES(const FGameplayAttributeData OldRES) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, RES, OldRES);
}

void UManequimAttributeSet::OnRep_VIG(const FGameplayAttributeData OldVIG) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, VIG, OldVIG);
}

/*
 *  Secondary Attributes Rep Notify Implementation
 */

void UManequimAttributeSet::OnRep_Armor(const FGameplayAttributeData OldArmor) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Armor, OldArmor);
}
void UManequimAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, ArmorPenetration, OldArmorPenetration);
}
void UManequimAttributeSet::OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, BlockChance, OldBlockChance);
}
void UManequimAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, CriticalHitChance, OldCriticalHitChance);
}
void UManequimAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}
void UManequimAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}
void UManequimAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, HealthRegeneration, OldHealthRegeneration);
}
void UManequimAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData OldManaRegeneration) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, ManaRegeneration, OldManaRegeneration);
}
void UManequimAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, MaxHealth, OldMaxHealth);
}
void UManequimAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, MaxMana, OldMaxMana);
}
void UManequimAttributeSet::OnRep_FireResistance(const FGameplayAttributeData OldFireResistance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, FireResistance, OldFireResistance);
}
void UManequimAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData OldLightningResistance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, LightningResistance, OldLightningResistance);
}
void UManequimAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData OldArcaneResistance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, ArcaneResistance, OldArcaneResistance);
}
void UManequimAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData OldPhysicalResistance) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, PhysicalResistance, OldPhysicalResistance);
}


/*
 *  Vital Attributes Rep Notify Implementation
 */

void UManequimAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Health, OldHealth);
}

void UManequimAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Mana, OldMana);
}




