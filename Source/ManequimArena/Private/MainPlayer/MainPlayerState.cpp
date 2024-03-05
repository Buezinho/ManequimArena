// Copyright to Bue.


#include "MainPlayer/MainPlayerState.h"
#include <AbilitySystem/ManequimAttributeSet.h>
#include <AbilitySystem/ManequimAbilitySystemComponent.h>

AMainPlayerState::AMainPlayerState()
{
	//For Enemies, we construct the ASC and AS on the character. For Players we construct them on the PlayerState
	AbilitySystemComponent = CreateDefaultSubobject<UManequimAbilitySystemComponent>("AbilitySystemComponent");
	//Ensure it's replicated
	AbilitySystemComponent->SetIsReplicated(true);
	//Players need the full gameplayeffect replication for their owned ASC
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UManequimAttributeSet>("AttributeSet");

	//Frequency that the server comunicates with clients
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
