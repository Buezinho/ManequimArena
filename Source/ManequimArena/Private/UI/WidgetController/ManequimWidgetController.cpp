// Copyright to Bue.


#include "UI/WidgetController/ManequimWidgetController.h"

/// <summary>
/// Called by our APlayerCharacter::InitAbilityActorInfo()
/// Sets our Widget and Fires the SetWidgetController Event on BluePrint
/// </summary>
/// <param name="WCParams"></param>
void UManequimWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UManequimWidgetController::BroadcastInitialValues()
{
	//empty on pourpuse. We will override this function on our Controllers implementations
}

void UManequimWidgetController::BindCallbacksToDependencies()
{
	//empty on pourpuse. We will override this function on our Controllers implementations
}
