// Copyright to Bue.


#include "ManequimAssetManager.h"
#include "ManequimGameplayTags.h"

UManequimAssetManager& UManequimAssetManager::Get()
{
	// TODO: insert return statement here

	check(GEngine);

	UManequimAssetManager* ManequimAssetManager = Cast<UManequimAssetManager>(GEngine->AssetManager);
	
	return *ManequimAssetManager;
}

/// <summary>
/// Overrides the StartInitialLoading function, which is called very early on to add our Tags initialization on the project
/// </summary>
void UManequimAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FManequimGameplayTags::InitializeNativeGameplayTags();
}
