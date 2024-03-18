// Copyright to Bue.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem//ManequimAbilitySystemComponent.h"
#include "AbilitySystem//ManequimAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "ManequimGameplayTags.h"

/// <summary>
/// Binds all of our attributeValueChangeDelegate to a Lambda Function that purpose is to Broadcast it's Attribute Info struct to our controllers
/// </summary>
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UManequimAttributeSet* AS = CastChecked<UManequimAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		//For each Tag map that we have, bind it's Function Pointer to a Lambda function
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

/// <summary>
/// Loops through all of our attributes Broadcasting all of them
/// </summary>
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UManequimAttributeSet* AS = CastChecked<UManequimAttributeSet>(AttributeSet);

	check(AttributeInfo);

	//Loop through our Map of GameplayTags to Attribute Delegate Functions
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());		
	}
}

/// <summary>
/// Broadcasts the attribute that is being called in Pair.Value() Function Pointer
/// </summary>
/// <param name="AttributeTag">our GameplayTag</param>
/// <param name="Attribute">the FunctionPointer to our Get[AttrName]Attribute function. To retrieve the attribute, we call Pair.Value()</param>
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FManequimAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	//And finally we can get the Numecric Value from the attribute
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);

	AttributeInfoDelegate.Broadcast(Info);
}


