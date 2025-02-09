// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ManequimBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ManequimAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "ManequimArena/ManequimArena.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AManequimBaseCharacter::AManequimBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Set the colision from Capsule and mesh to ignore the camera to avoid weird zoom-ins
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

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
/// Implements the ICombatInterface and returns the HitReact (microstun) Montage for this Character
/// </summary>
/// <returns></returns>
UAnimMontage* AManequimBaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AManequimBaseCharacter::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath();
}

/// <summary>
/// Handles what happens on both server and client on death
/// Runs on both Server and Client
/// </summary>
void AManequimBaseCharacter::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	//TODO: Instead of RagDoll, play a Death Animation
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();	
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

/// <summary>
/// Returns the location where our projectiles are going to spawn
/// </summary>
/// <returns></returns>
FVector AManequimBaseCharacter::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponProjectileSpawnSocketName);

	//TODO: Implement Unarmed Projectile Spawn
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

void AManequimBaseCharacter::AddCharacterAbilities()
{
	UManequimAbilitySystemComponent* ASC = CastChecked<UManequimAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
}


/// <summary>
/// Handles the Material swapping after death to "dissolve" enemies bodies
/// </summary>
void AManequimBaseCharacter::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMaterialInstance);
		StartDissolveTimeline(DynamicMaterialInstance);
		//TODO: Dissolvable objects can have multiple materials. This has to be implemented
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, WeaponDynamicMaterialInstance);
		StartWeaponDissolveTimeline(WeaponDynamicMaterialInstance);
		//TODO: Dissolvable objects can have multiple materials. This has to be implemented
	}
}



