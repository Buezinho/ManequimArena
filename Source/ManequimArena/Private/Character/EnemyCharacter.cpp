// Copyright to Bue.


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/ManequimAbilitySystemLibrary.h"
#include "AbilitySystem/ManequimAbilitySystemComponent.h"
#include "AbilitySystem/ManequimAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/ManequimUserWidget.h"
#include "ManequimArena/ManequimArena.h"
#include "ManequimGameplayTags.h"
#include "AI/ManequimAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	bReplicates = true;

	Level = 10;

	//For Enemies, we construct the ASC and AS on the character. For Players we construct them on the PlayerState
	AbilitySystemComponent = CreateDefaultSubobject<UManequimAbilitySystemComponent>("AbilitySystemComponent");
	//Ensure it's replicated
	AbilitySystemComponent->SetIsReplicated(true);
	//AI characters don't need to know the gameplayeffect replication
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//Set Character movement variables as default to ensure behaviour
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UManequimAttributeSet>("AttributeSet");

	//Instantiate our Health Bar and attach it to the root component
	//Position can be set on the Viewport
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Because AI must only run on the controller (all what the client see's is a replication), if we are not the server, return.
	if (!HasAuthority()) return;

	//When we possess the Enemy character with the AI controller, we can now set our AI Controller to our variable for better access in the future
	ManequimAIController = Cast<AManequimAIController>(NewController);

	ManequimAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	ManequimAIController->RunBehaviorTree(BehaviorTree);
}

/// <summary>
/// Implementation of ICombatInterface GetPlayerLevel function.
/// Returns the Enemy Level from the Level variable
/// </summary>
/// <returns></returns>
int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifespanAfterDeath);
	//TODO: Drop loot before die
	Super::Die();
}

void AEnemyCharacter::MulticastHandleDeath_Implementation()
{
	Super::MulticastHandleDeath_Implementation();

	//FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
	HealthBar->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}



void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//init Walking speed
	//TODO: move to BaseCharacter
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();

	//Because Abilities only can be given on the server, it's recommended to check if we have authority (we are the server) before trying to give abilities
	if (HasAuthority())
	{
		//Initializes our Enemy abilities from the CharacterClassInfo::CommonAbilities Data Asset
		UManequimAbilitySystemLibrary::GiveStartUpAbilities(this, AbilitySystemComponent);
	}
	
	

	//Before bind our delegates to our signatures and before broadcast our initial values, we have to set the HealthBar Widget controller to the Enemy Character
	//This way we guarantee that on our Event Graph, our widget controller is set.	
	if (UManequimUserWidget* Widget = Cast<UManequimUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		//As mentioned before, anything (UObject) can be a WidgetController as long as it provides all the data and signatures that the widget requires
		Widget->SetWidgetController(this);
	}

	//We can bind to our delegates after we initialize our AbilitySystemComponent
	//Instead of creating a new function that only broadcasts the value and bind to our GetGameplayAttributeValueChangeDelegate using AddUObject
	//We can explicitly add the lambda function that does the same (only broadcasts the value)
	UManequimAttributeSet* ManequimAS = Cast<UManequimAttributeSet>(AttributeSet);
	if (ManequimAS)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAS->GetHealthAttribute()).AddLambda(
			[this, ManequimAS](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
				OnMaxHealthChanged.Broadcast(ManequimAS->GetMaxHealth());
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		//Bind to the change in our Gameplay Tags (specifically when added)
		AbilitySystemComponent->RegisterGameplayTagEvent(FManequimGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::HitReactTagChanged);
		
		//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManequimAS->GetMaxHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnMaxHealthChangedDelegate);

		OnHealthChanged.Broadcast(ManequimAS->GetHealth());
		OnMaxHealthChanged.Broadcast(ManequimAS->GetMaxHealth());
	}

	//BroadcastInitialValues();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	//We need to inform our class that we initialized the Actor Info, so we can bind our Delegates
	Cast<UManequimAbilitySystemComponent>(GetAbilitySystemComponent())->AbilityActorInfoSet();

	//Because attributes are only given on the server, we must check if we have authority
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UManequimAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AEnemyCharacter::BroadcastInitialValues()
{
	//Cast our generic attribute set to our ManequimAttributeSet
	const UManequimAttributeSet* ManequimAS = CastChecked<UManequimAttributeSet>(AttributeSet);

	if (ManequimAS)
	{
		OnHealthChanged.Broadcast(ManequimAS->GetHealth());
		OnMaxHealthChanged.Broadcast(ManequimAS->GetMaxHealth());
	}
	
}

//void AEnemyCharacter::OnMaxHealthChangedDelegate(const FOnAttributeChangeData& Data)
//{
//	//OnMaxHealthChanged.Broadcast(Data.NewValue);
//}

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

/// <summary>
/// Callback Function that should be bound to AbilitySystemComponent->RegisterGameplayTagEvent()
/// Everytime the ASC receives a tag, this is called
/// </summary>
/// <param name="CallbackTag"></param>
/// <param name="NewCount"></param>
void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	//TODO: move to BaseCharacter
	bHitReacting = NewCount > 0;

	//If we are reacting to a hit, zero out our walking movement. Return to normal if not
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}
