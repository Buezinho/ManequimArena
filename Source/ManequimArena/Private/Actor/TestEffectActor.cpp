// Copyright to Bue.


#include "Actor/TestEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ManequimAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
ATestEffectActor::ATestEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());

}

void ATestEffectActor::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: change this to apply a GameplayEffect
	//For now using const cast as a Hack
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UManequimAttributeSet* AttributeSet = Cast<UManequimAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UManequimAttributeSet::StaticClass()));
		UManequimAttributeSet* MutableAttributeSet = const_cast<UManequimAttributeSet*>(AttributeSet);
		MutableAttributeSet->SetHealth(AttributeSet->GetHealth() + 25.f);
		MutableAttributeSet->SetMana(AttributeSet->GetMana() - 15.f);
		Destroy();
	}
}

void ATestEffectActor::EndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ATestEffectActor::BeginPlay()
{
	Super::BeginPlay();

	//Delegate the sphere overlap to OnOverlap in our class
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATestEffectActor::OnOverlap);
	//Delegate the sphere leaving overlap to EndOverlap in our class
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATestEffectActor::EndOverlap);
	
}


