// Copyright to Bue.


#include "AbilitySystem/ManequimAttributeSet.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UManequimAttributeSet::UManequimAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(120.f);
	InitMana(50.f);
	InitMaxMana(80.f);

}

void UManequimAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//In here we register variables for replication
	//COND_None = Replicate always / REPNOTIFY_Aways = always notify changes
	//For our simple game this will sufice
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManequimAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UManequimAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
		/*UE_LOG(LogTemp, Warning, TEXT("Health: %f"), NewValue);*/
	}
	//if (Attribute == GetMaxHealthAttribute())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Max Health: %f"), NewValue);
	//}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
		/*UE_LOG(LogTemp, Warning, TEXT("Mana: %f"), NewValue);*/
	}
	//if (Attribute == GetMaxManaAttribute())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Max Mana: %f"), NewValue);
	//}
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
		UE_LOG(LogTemp, Warning, TEXT("Health form GetHealth(): %f"), GetHealth());
		UE_LOG(LogTemp, Warning, TEXT("Magnitude from Data.EvaluatedData.Magnitude: %f"), Data.EvaluatedData.Magnitude);

	}
}

void UManequimAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Health, OldHealth);
}
void UManequimAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, MaxHealth, OldMaxHealth);
}
void UManequimAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, Mana, OldMana);
}
void UManequimAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const
{
	//We have to inform the Ability System that this attribute has changed 
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManequimAttributeSet, MaxMana, OldMaxMana);
}


