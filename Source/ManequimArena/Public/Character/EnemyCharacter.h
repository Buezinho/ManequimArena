// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "Character/ManequimBaseCharacter.h"
#include "Interaction/InteractableInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API AEnemyCharacter : public AManequimBaseCharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	
	AEnemyCharacter();

	/* Enemy Interface */
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	/* End of Enemy Interface */

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
};
