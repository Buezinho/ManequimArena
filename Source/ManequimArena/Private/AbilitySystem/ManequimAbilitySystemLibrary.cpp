// Copyright to Bue.


#include "AbilitySystem/ManequimAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer/MainPlayerState.h"
#include "UI/WidgetController/ManequimWidgetController.h"
#include "UI/HUD/ManequimHUD.h"
#include "AbilitySystemComponent.h"
#include "Game/ManequimArenaGameModeBase.h"
#include "ManequimAbilityTypes.h"

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

/// <summary>
/// Reads the Default Attributes from the UCharacterClassInfo Data Asset and Applies the Primary, Secondary and Vital GE's for that Class
/// </summary>
/// <param name="WorldContextObject"></param>
/// <param name="CharacterClass"></param>
/// <param name="Level"></param>
/// <param name="ASC"></param>
void UManequimAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	//Cast to our GameMode class type that contains the CharacterClassInfo Data Asset
	//AManequimArenaGameModeBase* ManequimGM = Cast<AManequimArenaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	//if (ManequimGM == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	//Get the Default Class Info for the desired class
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

	//Because we use MMC on some Gameplay Effects, we have to make sure we have a source object defined for our Spec.
	AActor* AvataActor = ASC->GetAvatarActor();	

	//Apply the GE's to the ASC	
	//1- Create the context Handle
	//2- Add Source Object to Context Handle, so we can Use it Later (especially for MMC's)
	//3- Create the Effect Spec Handle using our Context
	//4- Apply the Effect Spec
	FGameplayEffectContextHandle PrimayAttributesContexHandle = ASC->MakeEffectContext();
	PrimayAttributesContexHandle.AddSourceObject(AvataActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimayAttributesContexHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContexHandle = ASC->MakeEffectContext();
	SecondaryAttributesContexHandle.AddSourceObject(AvataActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContexHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContexHandle = ASC->MakeEffectContext();
	VitalAttributesContexHandle.AddSourceObject(AvataActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContexHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

}

/// <summary>
/// Gives the Startup Abilities from CharacterClassInfo Data Asset to the Character
/// </summary>
/// <param name="WorldContextObject"></param>
/// <param name="ASC"></param>
void UManequimAbilitySystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	//Cast to our GameMode class type that contains the CharacterClassInfo Data Asset
	//AManequimArenaGameModeBase* ManequimGM = Cast<AManequimArenaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	//if (ManequimGM == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UManequimAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	//Cast to our GameMode class type that contains the CharacterClassInfo Data Asset
	AManequimArenaGameModeBase* ManequimGM = Cast<AManequimArenaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ManequimGM == nullptr) return nullptr;

	return ManequimGM->CharacterClassInfo;
}

bool UManequimAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FManequimGameplayEffectContext* ManequimEffectContext = static_cast<const FManequimGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ManequimEffectContext->IsBlockedHit();
	}

	return false;
}

bool UManequimAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FManequimGameplayEffectContext* ManequimEffectContext = static_cast<const FManequimGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ManequimEffectContext->IsCriticalHit();
	}

	return false;
}


void UManequimAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FManequimGameplayEffectContext* ManequimEffectContext = static_cast<FManequimGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ManequimEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UManequimAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FManequimGameplayEffectContext* ManequimEffectContext = static_cast<FManequimGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ManequimEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
