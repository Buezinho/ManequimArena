// Copyright to Bue.


#include "Actor/BaseEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ManequimAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"


// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

// Called when the game starts or when spawned
void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

/// <summary>
/// Class that we use to apply an effect of type GameplayEffectClass to a Target Actor
/// </summary>
/// <param name="Target"></param>
/// <param name="GameplayEffectClass"></param>
void ABaseEffectActor::ApplyEfectToTarget(AActor* TargetActorToApplyEffect, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// Try to get the AbilitySystemInterface from our Target Actor. If it implements our interface, apply the effect
	/*IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	if (ASCInterface)
	{
		ASCInterface->GetAbilitySystemComponent();		
	}*/

	if (TargetActorToApplyEffect->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	//This Static Library function does exactly the same as we did with the ASCInterface, and returns exactly the same results, so it's a nice shortcut
	//Also, if our actor doesn't implement the IAbilitySystemInterface, by default it can look for the component in the actor, so it's even nicier!
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActorToApplyEffect);

	//If the overlaping class doesn't have a GameplayAbilitySystem, do nothing
	if (TargetASC == nullptr) return;
	//Make sure that our GameplayEffectClass is set!!!
	check(GameplayEffectClass);

	//Apply an effect to an ASC Target...
	//For that we need a GameplayEffectSpec and it's context handle
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	//EffectSpecHandle.Data.Get()->Def Derreferences the pointer to our actual Effect Spec
	//Ideally we don't change the duration policy on runtime, but we can check it
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	//If the effect is infinite, we need to keep track of it, and for that we store it in a map structure
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//Our Map should link the FActiveGameplayEffectHandle to our UAbilitySystemComponent* TargetASC
		//That way we can always track back to the actor and other Gameplay Ability System Component variables
		//We only store the FActiveGameplayEffectHandle if we intend to remove it later, so we can check our removal policy before storing it
		ActiveEffectHandlesMap.Add(ActiveEffectHandle, TargetASC);
		
	}

	//Only destroy effect actors that are not infinite!
	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}

}

void ABaseEffectActor::OnOverlap(AActor* TargetActorToApplyEffect)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEfectToTarget(TargetActorToApplyEffect, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEfectToTarget(TargetActorToApplyEffect, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEfectToTarget(TargetActorToApplyEffect, InfiniteGameplayEffectClass);
	}
}

void ABaseEffectActor::OnEndOverlap(AActor* TargetActorToApplyEffect)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEfectToTarget(TargetActorToApplyEffect, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEfectToTarget(TargetActorToApplyEffect, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEfectToTarget(TargetActorToApplyEffect, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//ApplyEfectToTarget(TargetActorToApplyEffect, DurationGameplayEffectClass);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActorToApplyEffect);
		if (!IsValid(TargetASC)) return;

		//loop through our TMap.
		//As we find our Effects to remove, instead of removing it in the loop, we add them to an Array to remove later
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHandlesMap)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);

			}
		}

		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandlesMap.FindAndRemoveChecked(Handle);
		}
	}
}


