// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ManequimAttributeSet.generated.h"

//struct FOnAttributeChangeData;

//struct FGameplayEffectModCallbackData;
//class FGameplayEffectModCallbackData;


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//Creates a Delegate that receives 0 arguments and returns an Attribute
//DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	//Source Properties
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	//Target Properties
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
};


//typedef is specific to the FGameplayAttribute() signature but Template is more versatile to any signature
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

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

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//Create a Map that maps a gameplayTag to a delegate function
	//TBaseStaticDelegateInstance is a generic function that binds to any pointer with a specific signature
	//In our case the signature receives 0 input params and returns a FGameplayAttribute and assigns it to a pointer
	//FAttributeFuncPtr is an ALIAS to TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	
	//Attributes must have an UPROPERTY()
	//Attributes must be Replicated
	//All replicated attributes must have a ReplicationNotify
	//Convention states that we must declare a function called OnRep_*AttributeName*
	//i.e. UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_*AttributeName*)
	//Register the notification type on GetLifetimeReplicatedProps for each variable

	/*
	 *  Primary Attributes Variable Declaration, Getters and Setters
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_STR, Category = "Primary Attributes")
	FGameplayAttributeData STR;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, STR);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_INT, Category = "Primary Attributes")
	FGameplayAttributeData INT;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, INT);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RES, Category = "Primary Attributes")
	FGameplayAttributeData RES;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, RES);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_VIG, Category = "Primary Attributes")
	FGameplayAttributeData VIG;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, VIG);

	/*
	 *  Secondary Attributes Variable Declaration, Getters and Setters
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Secondary Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Secondary Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Secondary Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Secondary Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, PhysicalResistance);

	/*
	 *  Vital Attributes Variable Declaration, Getters and Setters
	 */

	//Attributes declaration. All Attributes are of type FGameplayAttributeData
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	//Create Macros for GETTERS and SETTERS for each attribute
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Mana);

	/*
	 *  Meta Attributes Variable Declaration, Getters and Setters
	 */

	//Meta Attributes are NOT replicated and are used as a staging area to calculate real modifications to attributes on the SERVER
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UManequimAttributeSet, IncomingDamage);

	
	//Attributes Replication Functions declaration
	//In order to this functions to be notification, we need to add the UFUNCTION tag
	//OnRep can have either no value or a FGameplayAttributeData that passes the OLD value. This can be important for comparisons and calculations
	/*
	 *  Primary Attributes Rep Notify
	 */

	UFUNCTION()
	void OnRep_STR(const FGameplayAttributeData OldSTR) const;

	UFUNCTION()
	void OnRep_INT(const FGameplayAttributeData OldINT) const;

	UFUNCTION()
	void OnRep_RES(const FGameplayAttributeData OldRES) const;

	UFUNCTION()
	void OnRep_VIG(const FGameplayAttributeData OldVIG) const;

	/*
	 *  Secondary Attributes Rep Notify
	 */

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const; 

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData OldFireResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData OldLightningResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData OldArcaneResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData OldPhysicalResistance) const;


	/*
	 *  Vital Attributes Rep Notify
	 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData OldHealth) const;


	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData OldMana) const;


	//Create Macros for GETTERS and SETTERS for each attribute
	//ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Health);
	//ATTRIBUTE_ACCESSORS(UManequimAttributeSet, MaxHealth);
	//ATTRIBUTE_ACCESSORS(UManequimAttributeSet, Mana);
	//ATTRIBUTE_ACCESSORS(UManequimAttributeSet, MaxMana);

protected:

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const;

	void ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bBlockedHit, bool bCriticalHit);
};
