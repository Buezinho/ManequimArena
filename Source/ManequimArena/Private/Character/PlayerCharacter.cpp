// Copyright to Bue.


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ManequimAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainPlayer/MainPlayerState.h"
#include "MainPlayer/MainPlayerController.h"
#include "UI/HUD/ManequimHUD.h"


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
	InitAbilityActorInfo();
	
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);

	MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
	//We need to inform our class that we initialized the Actor Info, so we can bind our Delegates
	Cast<UManequimAbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = MainPlayerState->GetAbilitySystemComponent();
	AttributeSet = MainPlayerState->GetAttributeSet();

	//The controller is always accessible from the player with the GetController() function
	//Because only the server has all valid controllers (Remember that the player only possesses his controller), we need to validate if we have a valid player controller.
	//We will only init our own HUD, so that's OK to ignore all invalid Player Controllers
	if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetController()))
	{
		//The controller always can access the HUD
		//As the controller, the player only has access to his HUD, so we have to assert if
		if (AManequimHUD* HUD = Cast<AManequimHUD>(MainPlayerController->GetHUD()))
		{
			//This will init our Overlay Widgets, Widget Controller, Init it and Add to Viewport
			HUD->InitOverlay(MainPlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

}