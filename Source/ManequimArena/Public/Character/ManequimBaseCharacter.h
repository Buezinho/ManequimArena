// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
//#include "AbilitySystem/ManequimAbilitySystemComponent.h"
//#include "AbilitySystem/ManequimAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "ManequimBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;

UCLASS(Abstract)
class MANEQUIMARENA_API AManequimBaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AManequimBaseCharacter();
	//Implement the AbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//--------------------------------------------------------------------------
	// Combat Functionalities
	//--------------------------------------------------------------------------
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	//--------------------------------------------------------------------------
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	//--------------------------------------------------------------------------
	// Combat Functionalities
	//--------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponProjectileSpawnSocketName;

	virtual FVector GetCombatSocketLocation() override;

	//--------------------------------------------------------------------------

	//Pointers to ASC and AS to be instantiated by the character for Enemies and by the PlayerState in the Players
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	//--------------------------------------------------------------------------
	// Attribute Functionalities
	//--------------------------------------------------------------------------

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	//This effect is intended to set (override) all of our primary attributes. It's a nice alternative to initialize attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	virtual void InitializeDefaultAttributes() const;

	//--------------------------------------------------------------------------
	// Combat Functionalities
	//--------------------------------------------------------------------------

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level) const;
	

	void AddCharacterAbilities();

	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	// Dissolve Effects
	//--------------------------------------------------------------------------

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	//--------------------------------------------------------------------------

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray <TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
