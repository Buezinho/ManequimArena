// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ManequimBaseCharacter.h"

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

UAbilitySystemComponent* AManequimBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AManequimBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}



