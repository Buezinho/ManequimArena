// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManequimUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API UManequimUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	//All widgets must have a controller
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	//As soon as the WidgetController is set, we want to initialize this Widget
	//This will be called as soon as we set an WidgetController
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
