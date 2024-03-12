// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IInteractableInterface;

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

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerInputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	/// <summary>
	/// Traces a ray and check colisions under the cursor
	/// </summary>
	void CursorTrace();

	//Create pointers for last and current interactables, so we cal call properly highlight and unHighlight
	//TObjectPtr<IInteractableInterface> LastActor;
	//TObjectPtr<IInteractableInterface> CurrentActor;
	IInteractableInterface* LastActor;
	IInteractableInterface* CurrentActor;
	
};
