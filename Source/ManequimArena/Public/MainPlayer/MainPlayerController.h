// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "MainPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IInteractableInterface;
class UManequimInputConfig;
class UManequimAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;



/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY()
	TObjectPtr<UManequimAbilitySystemComponent> ManequimAbilitySystemComponent;

	UManequimAbilitySystemComponent* GetASC();

	


	//-----------------------------------------------
	//Variables for the Cursor functionality
	//-----------------------------------------------

	void CursorTrace();
	//Create pointers for last and current interactables, so we cal call properly highlight and unHighlight
	IInteractableInterface* LastActor;
	IInteractableInterface* CurrentActor;
	FHitResult CursorHit;

	//-----------------------------------------------

	//-----------------------------------------------
	// INPUT Variables 
	//-----------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerInputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UManequimInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	//Callback functions
	void Move(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;
	
	//-----------------------------------------------

	//-----------------------------------------------
	//Variables for the Click-to-Move functionality
	//-----------------------------------------------

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	float MovementScale = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	//-----------------------------------------------

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	
};
