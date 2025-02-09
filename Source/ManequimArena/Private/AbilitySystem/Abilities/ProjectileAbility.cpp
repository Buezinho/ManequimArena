// Copyright to Bue.


#include "AbilitySystem/Abilities/ProjectileAbility.h"
#include "Actor/ProjectileActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "ManequimGameplayTags.h"


void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	
	//Check if the server is calling Activate ability... if not do nothing
	//It's a recomended practice to guarantee that we only activate abilities on the server side and then replicate to the clients
	//const bool bIsServer = HasAuthority(&ActivationInfo);
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	//We need the Combat interface so we can call GetCombatSocketLocation()
	//Remember that the avatar always points to the Character. The Owning Actor can be either the Character or the Controller
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector ProjectileSpawnSocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - ProjectileSpawnSocketLocation).Rotation();
		//We zero our pitch so the projectile always goes parallel to the ground
		//This is a gameplay decision. we won't implement gravity, so that's ok
		//Rotation.Pitch = 0.f;
		//For now we will not zero out the pitch because there's a delay between server and client, and clients are shooting a little bit high the projectile

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(ProjectileSpawnSocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());


		//SpawnActorDeferred allows us to delay the actual spawn and set properties such as GameplayEffect to it
		AProjectileActor* Projectile = GetWorld()->SpawnActorDeferred<AProjectileActor>(
			ProjectileActorClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give the projectile a Gameplay Effect Spec for causing damage
		//Create our SpecHandle so we can specify it on the projectile at spawn
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());	
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		//Set our context information
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult; //We know that for this spell it's empty
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);


		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		const FManequimGameplayTags GameplayTags = FManequimGameplayTags::Get();

		

		//Loop through our Ability Map and call the AssignTagSetByCallerMagnitude for each damage type on the Ability DamageTypes map
		for (auto& Pair : DamageTypes)
		{
			//Get the Curve value for our AbilityLevel for that type
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel()/*10.f*/);

			//This function assigns the Key Tag|Magnitude for a SpecHandle
			//This spell should apply any value we set on the magnitude as long as the GE uses this tag
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);

		}

		Projectile->Instigator = GetAvatarActorFromActorInfo();
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}

}
