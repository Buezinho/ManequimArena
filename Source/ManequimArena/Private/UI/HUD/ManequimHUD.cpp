// Copyright to Bue.


#include "UI/HUD/ManequimHUD.h"

#include "UI/Widget/ManequimUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

/// <summary>
/// Singleton-ish function to initialize our Overlay Widget Controller
/// WCParams is a struct that expects APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS
/// </summary>
/// <param name="WCParams"></param>
/// <returns></returns>
UOverlayWidgetController* AManequimHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		//Binding the CallBacks means we delegate the Attribute changes and broadcast it to our widgets, that listen to our controller and make the appropriated changes
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
	
}

/// <summary>
/// Inits the User Widget and and Widget Controller.
/// For the controller we need all 4 Params
/// </summary>
/// <param name="PC"></param>
/// <param name="PS"></param>
/// <param name="ASC"></param>
/// <param name="AS"></param>
void AManequimHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//In Intit Overlay we have to create both the Widget and WidgetController
	
	//Check if our classes are set properly in BluePrint
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_ManequimHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_ManequimHUD"));

	//Instantiate a new Widget of type OverlayWidget <UManequimUserWidget> 
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	//Cast our UserWidget to ManequimUserWidget
	OverlayWidget = Cast<UManequimUserWidget>(Widget);

	//Intantiate a new Widget Controler of type OverlayWidgetController <UOverlayWidgetController : UManequimWidgetController>
	//For that we need to create a struct of type FWidgetControllerParams to initialize our controller
	//Remember that our controller needs those models to properlly receive all notifications necessary for our game
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	//Now we can contruct our Widget Controller using our "singleton-ish" function
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	//Remember that ALL User Widgets must have a controller assigned, so we need to set our controller in all Widgets
	OverlayWidget->SetWidgetController(WidgetController);

	//We can only start broadcasting our values after the widget controller is set. This will be broadcasted even if we didn't have any changes!!!
	WidgetController->BroadcastInitialValues();

	//Add it to our ViewPort
	Widget->AddToViewport();

}

UAttributeMenuWidgetController* AManequimHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);

		//Binding the CallBacks means we delegate the Attribute changes and broadcast it to our widgets, that listen to our controller and make the appropriated changes
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

//void AManequimHUD::InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
//{
//	//Check if our classes are set properly in BluePrint
//	checkf(AttributeMenuWidgetClass, TEXT("Attribute Menu Widget Class uninitialized, please fill out BP_ManequimHUD"));
//	checkf(AttributeMenuWidgetControllerClass, TEXT("Attribute Menu Widget Controller Class uninitialized, please fill out BP_ManequimHUD"));
//
//	//Instantiate a new Widget of type OverlayWidget <UManequimUserWidget> 
//	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AttributeMenuWidgetClass);
//	//Cast our UserWidget to ManequimUserWidget
//	AttributeMenuWidget = Cast<UManequimUserWidget>(Widget);
//}

void AManequimHUD::BeginPlay()
{
	Super::BeginPlay();
}
