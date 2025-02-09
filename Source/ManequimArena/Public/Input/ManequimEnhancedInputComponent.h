// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "ManequimInputConfig.h"
#include "ManequimEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UManequimEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UManequimInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UManequimEnhancedInputComponent::BindAbilityActions(const UManequimInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FAbilityInputAction& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				//On the Bind Action Function, all parameters after our Func will be passed as input parameters to the Func itself
				//That's why we pass the inputTag so we can know which ability we will activate
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				//On the Bind Action Function, all parameters after our Func will be passed as input parameters to the Func itself
				//That's why we pass the inputTag so we can know which ability we will activate
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				//On the Bind Action Function, all parameters after our Func will be passed as input parameters to the Func itself
				//That's why we pass the inputTag so we can know which ability we will activate
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
			
		}
	}


}
