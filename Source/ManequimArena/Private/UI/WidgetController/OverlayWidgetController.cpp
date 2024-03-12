// Copyright to Bue.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem//ManequimAbilitySystemComponent.h"
#include "AbilitySystem//ManequimAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
//#include "GameplayTagContainer.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//Cast our generic attribute set to our ManequimAttributeSet
	const UManequimAttributeSet* ManequimAttributeSet = CastChecked<UManequimAttributeSet>(AttributeSet);
	
	OnHealthChange.Broadcast(ManequimAttributeSet->GetHealth());
	OnMaxHealthChange.Broadcast(ManequimAttributeSet->GetMaxHealth());
	OnManaChange.Broadcast(ManequimAttributeSet->GetMana());
	OnMaxManaChange.Broadcast(ManequimAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UManequimAttributeSet* ManequimAttributeSet = CastChecked<UManequimAttributeSet>(AttributeSet);

	//This Binds our local HealthChanged function to the delegate from AbilitySystemComponent for when the Health Attribute changes
	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
	/*******  WE WILL REPLACE THE TRADITIONAL DELEGATE CALLBACKS WITH LAMBDA. THEY ARE SIMPLE ENOUGH TO DO SO... NOT GETTING RID OF THE ORIGINAL CODE  *******/
	/*AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) 
		{
			OnHealthChange.Broadcast(Data.NewValue);
		}
	);*/

	//Bind our Tags
	//Lambda Functions are generic functions that are handfull when we don't want to create a specific function for that
	//Lambda Functions are handy for simple Functions that can contain all the code within and don't require to create Functions to implement simple code
	Cast<UManequimAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) 
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False. 
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				

				//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

				if (Tag.MatchesTag(MessageTag))
				{
					//Get the row from our data table
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);

				}	
			}
		}
	);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	//When an attribute changes, we listen using this *Attribute*Changed function, and we need to broadcast to our widgets.
	// Our Widgets already listen to our broadcast...
	//Remember that this is the Widget Controller, so it's main function is to listen from the models, and inform the changes to the views
	//OnHealthChange.Broadcast(Data.NewValue);

}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	//When an attribute changes, we listen using this *Attribute*Changed function, and we need to broadcast to our widgets
	//Remember that this is the Widget Controller, so it's main function is to listen from the models, and inform the changes to the views
	OnMaxHealthChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	//When an attribute changes, we listen using this *Attribute*Changed function, and we need to broadcast to our widgets
	//Remember that this is the Widget Controller, so it's main function is to listen from the models, and inform the changes to the views
	OnManaChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	//When an attribute changes, we listen using this *Attribute*Changed function, and we need to broadcast to our widgets
	//Remember that this is the Widget Controller, so it's main function is to listen from the models, and inform the changes to the views
	OnMaxManaChange.Broadcast(Data.NewValue);
}
