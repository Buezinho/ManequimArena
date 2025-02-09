// Copyright to Bue.


#include "Actor/ProjectileActor.h"
#include "ManequimArena/ManequimArena.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
//#include "../../../../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// As this will be spawned on the server side, we need it to be replicated to the clients
	bReplicates = true;

	//--------------------------------------------------------------------------
	//Instantiate our Sphere
	//--------------------------------------------------------------------------
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Block all events except for World Dynamics, World Statics and Pawns
	//Any other type should not trigger the collision
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	//Instantiate our Projectile Movement
	//--------------------------------------------------------------------------
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	//--------------------------------------------------------------------------
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Lifespan);
	//Bind our OnBeginOverlap to our local function that will be implemented on BluePrint
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	
}

void AProjectileActor::Destroyed()
{
	// if we destroy the actor before the overlap event and this is NOT the server, play the effects before destroying
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if(LoopingSoundComponent) LoopingSoundComponent->Stop();
	}
	
	Super::Destroyed();
}

void AProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Instigator) return;

	//To avoid multiple instances of the sound, we have to check if bHit was not set (= false) before spawning the sound (same as when we are destroying it)

	if (!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}
	

	//There are some rare ocasions where the server will destroy the actor before the client receives the OnPverlap Event.
	//In those cases we can set the an internal variable bHit, and check OnDestroy if this variable is true or false
	//If it's true, we assume the OnOverlap event was already called on the client, so do Nothing
	//If it's false on the OnDestroy override, we have to play the effects before destroying the actor
	if (HasAuthority())
	{
		//Before Destroying the projectile on server, we need to apply our effect
		//if the actor we colide with has an AbilitySystemComponent, apply the effect
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

