// Copyright to Bue.


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainPlayer/MainPlayerState.h"


APlayerCharacter::APlayerCharacter()
{
	//Set the player to face the movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Set the rotation speed to a hardcoded value of 400
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//Constraint the player controler to the plane.... no elevation at this point
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//Standard configuration to avoid using the controller itself to move and rotate the player.
	//Instead, we define movement by moving the player IN the player controller at AMainPlayerController::Move
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitbilityActorInfo();
	
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitbilityActorInfo();
}

void APlayerCharacter::InitbilityActorInfo()
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);

	MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);

	AbilitySystemComponent = MainPlayerState->GetAbilitySystemComponent();
	AttributeSet = MainPlayerState->GetAttributeSet();
}