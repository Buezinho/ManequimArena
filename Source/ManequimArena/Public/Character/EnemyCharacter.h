// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "Character/ManequimBaseCharacter.h"
#include "Interaction/InteractableInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemyCharacter.generated.h"

//struct FOnAttributeChangeData;
class UWidgetComponent;
class UBehaviorTree;
class AManequimAIController;

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API AEnemyCharacter : public AManequimBaseCharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	
	AEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

	/* Enemy Interface */
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	/* End of Enemy Interface */

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	virtual void MulticastHandleDeath_Implementation() override;
	/* End of Combat Interface */

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifespanAfterDeath = 5.f;

	//As we will set the Enemy character its own widget controlle for the health bar
	//Create an Attribute Changed Signature to delagate our health and MaxHealth changes
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	//Create a Callback function for our Hit React. In this case we won't use Lambda
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	void BroadcastInitialValues();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults");
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults");
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI");
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY();
	TObjectPtr<AManequimAIController> ManequimAIController;
	

	
private:
	/*UFUNCTION(BlueprintCallable)
	void OnMaxHealthChangedDelegate(const FOnAttributeChangeData& Data);*/

};
