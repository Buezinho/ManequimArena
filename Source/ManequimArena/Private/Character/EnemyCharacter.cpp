// Copyright to Bue.


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/ManequimAbilitySystemComponent.h"
#include "AbilitySystem/ManequimAttributeSet.h"
#include "ManequimArena/ManequimArena.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//For Enemies, we construct the ASC and AS on the character. For Players we construct them on the PlayerState
	AbilitySystemComponent = CreateDefaultSubobject<UManequimAbilitySystemComponent>("AbilitySystemComponent");
	//Ensure it's replicated
	AbilitySystemComponent->SetIsReplicated(true);
	//AI characters don't need to know the gameplayeffect replication
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UManequimAttributeSet>("AttributeSet");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();


	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

}

void AEnemyCharacter::HighLightActor()
{
	bHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::UnHighLightActor()
{
	bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
