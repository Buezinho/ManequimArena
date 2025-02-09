// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ManequimArenaGameModeBase.generated.h"

class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API AManequimArenaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
