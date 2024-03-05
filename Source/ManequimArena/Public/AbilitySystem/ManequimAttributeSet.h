// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ManequimAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UManequimAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UManequimAttributeSet();
	//Required for replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	//Attributes must have an UPROPERTY()
	//Attributes must be Replicated
	//All replicated attributes must have a ReplicationNotify
	//Convention states that we must declare a function called OnRep_*AttributeName*
	//i.e. UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_*AttributeName*)
	//Register the notification type on GetLifetimeReplicatedProps for each variable

	//Attributes declaration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;

	//Attributes Replication Functions declaration
	//In order to this functions to be notification, we need to add the UFUNCTION tag
	//OnRep can have either no value or a FGameplayAttributeData that passes the OLD value. This can be important for comparisons and calculations
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const;

	//Create Macros for GETTERS and SETTERS for each attribute
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, MaxMana);

	
};
