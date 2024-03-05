// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "Character/ManequimBaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API APlayerCharacter : public AManequimBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:

	void InitbilityActorInfo();
	
	
};
