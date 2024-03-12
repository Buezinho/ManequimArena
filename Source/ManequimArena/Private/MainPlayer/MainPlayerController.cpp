// Copyright to Bue.

#include "MainPlayer/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include <Interaction/InteractableInterface.h>

AMainPlayerController::AMainPlayerController()
{
	//Make sure that this controller replicates for multi-player
	bReplicates = true;
}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Run a trace from the camera to check what the mouse is hovering and hilight the items that implement IInteractableInterface
	CursorTrace();
}

/// <summary>
/// Get the trace under the cursor
/// </summary>
void AMainPlayerController::CursorTrace()
{
	FHitResult cursorHit;
	//Check simple colisions against everything that is visible
	GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	
	if (!cursorHit.bBlockingHit) return;

	//Store the latest result from the raytrace. If the raytrace hits noting that is interactable, the result will be null
	LastActor = CurrentActor;
	//Check if the hit object implements IInteractableInterface, and asign it to currentActor
	CurrentActor = Cast<IInteractableInterface>(cursorHit.GetActor());

	/**
	* Line trace from cursor. There are severar scenarios:
	*   A) LastActor is null && CurrentActor is null
	*		- do nothing
	*	B) LastActor is null && CurrentActor is valid
	*		- Highlight CurrentActor
	*	C) LastActor is valid && CurrentActor is null
	* 		- UnHighlight CurrentActor
	*	D) both actors are valid, but LastActor != CurrentActor
	* 		- UnHighlight CurrentActor
	*	E) both actors are valid, but LastActor == CurrentActor
	* 		- do nothing
	**/

	if (LastActor == nullptr)
	{
		if (CurrentActor != nullptr)
		{
			// Case B
			CurrentActor->HighLightActor();
		}
		else
		{
			// Case A - Do Nothing
		}
	}
	else
	{
		if (CurrentActor == nullptr)
		{
			// Case C
			LastActor->UnHighLightActor();
		}
		else //both actor are valid
		{
			if (LastActor != CurrentActor)
			{
				// Case D
				LastActor->UnHighLightActor();
				CurrentActor->HighLightActor();
			}
			else
			{
				// Case E - Do Nothing
			}
		}

	}

}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();


	//Validate if our PlayerInputContext is valid
	check(PlayerInputContext);

	//Get the local player's singleton subsystem for our enhanced Input Subsystem.
	//This is what we add and remove inputs from our players
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	//Instead of check subsystem, we need to assert. This is because only the Hosting players will have access to all subsystems, so this can crash our game!!!
	//check(Subsystem);
	//This will work on multiplayer
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerInputContext, 0);
	}
	//Set the mouse cursor on begin play
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//Input game mode struct for mouse and keyboard
	FInputModeGameAndUI InputModeData;
	//We don't want the mouse to be locked to our game viewport 
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//We don't want our mouse to be hidden during capture
	InputModeData.SetHideCursorDuringCapture(false);
	
	//Sets our recently created input mode to the controller
	SetInputMode(InputModeData);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	//Access the input component
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	//Configure the MoveAction variable to receive the inputData related to our movement
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);


}

void AMainPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//Create a local vector to receive our input action from MoveAction
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	//In order to move the player, we have to find the Forward and Right vectors related to the camera.
	//As it's isometric, we can only consider the Yaw (x) rotation for that
	//After that we can get both the Forward and Right Directions normalized using the FRotationMatrix(YawRotation).GetUnitAxis

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//Get our Pawn controlled by this controller
	//As this is called by tick, sometimes we have a controller, but not yet a pawn, so we can add an IF statement to solve this
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		//The inputActionValue returned from our Enhanced input uses the (press W) Y = 1 for moving forward and (press S) Y = -1 for moving backwards
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		//Same thing goes to the press A and D for the Right/Left direction
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}


	
}

