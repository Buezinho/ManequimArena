// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ManequimBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// Sets default values
AManequimBaseCharacter::AManequimBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the Weapon Mesh Placeholder1
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	//Always Attach this weapon to the Hand Socket
	//Set's up the Weapon Mesh to our Character Base Mesh into the pre-defined WeaponHandSocket Socket
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	//Always remove the weapon colision, since only the player capsule should have colisions
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

/// <summary>
/// As our Base Character implements IAbilitySystemInterface, we have to implement this function
/// </summary>
/// <returns></returns>
UAbilitySystemComponent* AManequimBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/// <summary>
/// Called when the game starts or when spawned
/// </summary>
void AManequimBaseCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AManequimBaseCharacter::InitAbilityActorInfo()
{
}

void AManequimBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	//It's good practice to assure we are calling this function when both AbilitySystemComponent and DefaultPrimaryAttributes are set
	//If not, it's ok to crash the game
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	//We had to add the source object to make sure, that when we apply the Default Attributes, it's set. We need it to Calculate Atributes using ModMagnitudeCalculation (MMC)
	ContextHandle.AddSourceObject(this); //We pass the character as source because it's the one who implements the CombatInterface in both Enemy and Player Characters
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, 1.f, ContextHandle);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AManequimBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);

}



