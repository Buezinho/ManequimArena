// Copyright to Bue.


#include "UI/Widget/ManequimUserWidget.h"

void UManequimUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	//Everytime we assign a WidgetController, we want to initialize this Widget
	//This way, when we implement WidgetControllerSet in our Blueprint, we know that it is set
	WidgetControllerSet();
}
