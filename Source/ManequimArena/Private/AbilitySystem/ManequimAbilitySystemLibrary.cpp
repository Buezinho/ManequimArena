// Copyright to Bue.


#include "AbilitySystem/ManequimAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer/MainPlayerState.h"
#include "UI/WidgetController/ManequimWidgetController.h"
#include "UI/HUD/ManequimHUD.h"

/// <summary>
/// We expect this to be called from within a Widget. Widgets only exists on the perspective of a Local Player.
/// </summary>
/// <param name="WorldContextObject"></param>
/// <returns></returns>
UOverlayWidgetController* UManequimAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AManequimHUD* ManequimHUD = Cast<AManequimHUD>(PC->GetHUD()))
		{
			AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			//To get our Overlay Widget Controller we need to make sure that we have all info that it uses, so it's getter requires us to pass it:
			//The Player Controller, Player State, Ability System Component and Attibute Set
			//With those information at hand, we create a wrapping structure called FWidgetControllerParams that we created and them we can get our Overlay Widget Controller
			const  FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return ManequimHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

/// <summary>
/// We expect this to be called from within a Widget. Widgets only exists on the perspective of a Local Player.
/// </summary>
/// <param name="WorldContextObject"></param>
/// <returns></returns>
UAttributeMenuWidgetController* UManequimAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AManequimHUD* ManequimHUD = Cast<AManequimHUD>(PC->GetHUD()))
		{
			AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			//To get our Overlay Widget Controller we need to make sure that we have all info that it uses, so it's getter requires us to pass it:
			//The Player Controller, Player State, Ability System Component and Attibute Set
			//With those information at hand, we create a wrapping structure called FWidgetControllerParams that we created and them we can get our Overlay Widget Controller
			const  FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return ManequimHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}
