// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ManequimAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UManequimAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UManequimAssetManager& Get();
	
protected:
	virtual void StartInitialLoading();
};
