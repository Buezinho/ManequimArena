// Copyright to Bue.


#include "MainPlayer/MainPlayerState.h"
#include <AbilitySystem/ManequimAttributeSet.h>
#include <AbilitySystem/ManequimAbilitySystemComponent.h>
#include "Net/UnrealNetwork.h"

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

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Mark as replicated
	DOREPLIFETIME(AMainPlayerState, Level);
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainPlayerState::OnRep_Level(int32 OldLevel)
{
}
