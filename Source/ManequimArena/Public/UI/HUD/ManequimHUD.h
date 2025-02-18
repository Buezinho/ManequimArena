// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ManequimHUD.generated.h"

class UManequimUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API AManequimHUD : public AHUD
{
	GENERATED_BODY()

public:

	//Pointer to our single overlay widget controller
	//The idea is to make it something like a singleton and guarantee that we only have 1 Overlay Widget Controller at a time

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	/*void InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);*/

protected:

	virtual void BeginPlay() override;

private:

	//Pointer to our single overlay widget
	UPROPERTY()
	TObjectPtr<UManequimUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "WidgetController")
	TSubclassOf<UManequimUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "WidgetController")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	/*UPROPERTY(EditAnywhere, Category = "WidgetController")
	TSubclassOf<UManequimUserWidget> AttributeMenuWidgetClass;*/

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "WidgetController")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
};
