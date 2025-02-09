// Copyright to Bue.

#include "MainPlayer/MainPlayerController.h"

#include "EnhancedInputSubsystems.h"
//#include "EnhancedInputComponent.h"
//#include "InputTriggers.h"
#include "Components/SplineComponent.h"
#include "Interaction/InteractableInterface.h"
#include "Input/ManequimEnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ManequimAbilitySystemComponent.h"
//#include "GameplayTagContainer.h"
#include "ManequimGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "UI/Widget//DamageTextComponent.h"


AMainPlayerController::AMainPlayerController()
{
	//Make sure that this controller replicates for multi-player
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Run a trace from the camera to check what the mouse is hovering and hilight the items that implement IInteractableInterface
	//As this is checked every frame, we can store it's value to use later
	CursorTrace();

	AutoRun();
	
}

void AMainPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	//IsLocalController check guarantees that the server won't execute this method when called if he's not the controller.
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		//When we attach to the root component, we want the TargetCharacter position, but as we don't want the floating text to follow the Target, we detach it right away
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

/// <summary>
/// Binds our Callback functions for input keys
/// </summary>
void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Access the input component
	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	UManequimEnhancedInputComponent* EnhancedInputComponent = CastChecked<UManequimEnhancedInputComponent>(InputComponent);

	//Configure the MoveAction variable to receive the inputData related to our movement
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);
	//Configure the Generic Input Config for abilities activation. We also handle movement here
	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	//Bind the Shift callback function to identify if we have the Shift key pressed!!! We could have used Lambda as well
	//This way we bind and keep our bShiftKeyDown variable in sync with the KeyAction started and completed
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AMainPlayerController::ShiftPressed);
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AMainPlayerController::ShiftReleased);


}

void AMainPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	//This handles the AUTO MOVEMENT if we have a spline
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

/// <summary>
/// Get the trace under the cursor
/// </summary>
void AMainPlayerController::CursorTrace()
{
	
	//Check simple colisions against everything that is visible
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) return;
	//Store the latest result from the raytrace. If the raytrace hits noting that is interactable, the result will be null
	LastActor = CurrentActor;
	//Check if the hit object implements IInteractableInterface, and asign it to currentActor
	CurrentActor = Cast<IInteractableInterface>(CursorHit.GetActor());

	/**
	* Line trace from cursor. There are severar scenarios:
	*   A) LastActor == CurrentActor
	*		- do nothing
	*	B) LastActor != CurrentActor and LastActor is Valid
	*		- UnHighlight Last
	*	C) LastActor != CurrentActor and CurrentActor is Valid
	* 		- Highlight CurrentActor
	**/

	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnHighLightActor();
		if (CurrentActor) CurrentActor->HighLightActor();
	}



}

/// <summary>
/// Handles the OnKeyPressed for our InputConfig Data Asset
/// This function simply sets the variables... We handle the actual InputKey processing in Held and Released
/// </summary>
/// <param name="InputTag">Tag Assotiated with an InputAction</param>
void AMainPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//Check if the InputTag is our MovementTag (RMB)
	if (InputTag.MatchesTagExact(FManequimGameplayTags::Get().InputTag_RMB))
	{
		//Check if we are hovering any enemy. If so, mark it as target
		bTargeting = CurrentActor ? true : false;
		//As this is called in the first frame of the button pressed, we don't know yet if it's a short (auto) run or a follow the held mouse movement
		//So we set the AutoRun to false and decide later depending on the FollowTime
		bAutoRunning = false;
	}
	
}

/// <summary>
/// Handles the OnKeyReleased for our InputConfig Data Asset
/// Checks if our Key is movement and if we are targeting any enemy. Otherwise performs the Auto Movement using Splines
/// </summary>
/// <param name="InputTag">Tag Assotiated with an InputAction</param>
void AMainPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//If it's not the RMB, try activate the ability and leave
	if (!InputTag.MatchesTagExact(FManequimGameplayTags::Get().InputTag_RMB))
	{
		//Because we can call this too early, we have to check it
		if (GetASC())
		{
			//This means that it's up to the ASC itself to handle the inputs... not the controller
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	//We always inform that we released the mouse
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	//But if it`s not targeting nor Shift is being pressed... AND it`s a short press, we should AutoRun
	if (!bTargeting && !bShiftKeyDown)
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime < ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				//Reset Our Spline
				Spline->ClearSplinePoints();
				for (const FVector& PointLocation : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
				}
				//Sometime we can click on an unreachable area, so the destination will never be on our Spline, so we can override the destination to the last point of our spline
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				//If we have a path, we can set autoRunning to True
				bAutoRunning = true;
			}
		}
		//As we released our button, we can reset our auxiliary variables
		bTargeting = false;
		FollowTime = 0.f;
	}
}

/// <summary>
/// Handles the OnKeyHeld for our InputConfig Data Asset
/// Checks if our Key is movement and if we are targeting any enemy. Otherwise performs the Follow the mouse direction Movement 
/// </summary>
/// <param name="InputTag">Tag Assotiated with an InputAction</param>
void AMainPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//If it's not the RMB, try activate the ability and leave
	if (!InputTag.MatchesTagExact(FManequimGameplayTags::Get().InputTag_RMB))
	{
		//Because we can call this too early, we have to check it
		if (GetASC())
		{
			//This means that it's up to the ASC itself to handle the inputs... not the controller
			GetASC()->AbilityInputTagHeld(InputTag);
		}		
		return;
	}

	//If it's the RMB AND it's targeting OR we are holding shift, also try activate the ability
	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	//Otherwise, we do the movement code
	else
	{
		//Add the delta seconds for each frame to the follow time so we can check if it was a Short Press
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.Location;
		}

		//In order to call AddMovement, we need a valid Pawn
		if (APawn* ControlledPawn = GetPawn())
		{
			//As this is a Direction, we need it normalized... A direction to follow can be found by subtracting the vectors
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection, MovementScale);
		}
	}	
}

/// <summary>
/// Provides a single function to get and convert our ASC to our Project ASC so we can use the built in functions
/// </summary>
/// <returns></returns>
UManequimAbilitySystemComponent* AMainPlayerController::GetASC()
{
	if (ManequimAbilitySystemComponent == nullptr)
	{
		ManequimAbilitySystemComponent = Cast<UManequimAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return ManequimAbilitySystemComponent;
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

