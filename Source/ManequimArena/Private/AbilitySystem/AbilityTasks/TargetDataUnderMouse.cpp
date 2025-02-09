// Copyright to Bue.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//Super is not necessary, since all it does on GameplayAbilityTask is an UE_VLOG

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		//TODO: We are on the server, so listen for target data
		//Bind our server Delegate for when the Target Data is Set on Server to call our locally created function
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		//It may happen in some cases that the TargetData is sent BEFORE the Ability Activation (Lag, Package Loss, etc.)
		//And we can check if the Target data has already been sent to the server (is set at the server)
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		//If the targetData is already set in the server and Activation isn't, we must inform the server to wait for it
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();

		}

	}

	
}

/// <summary>
/// If we are locally controlled, this function broadcasts the delegate and sends the data to the server
/// If we are on the server, we broadcast
/// </summary>
void UTargetDataUnderMouse::SendMouseCursorData()
{
	//In order to send TargetDataHandle to the server we need to have prediction Keys
	//Prediction Keys must have an window where it can do things locally and predict the server response
	//This will allow us to do everything under this scope locally and server will only validate later
	//If done right, it help games to be smooth.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	FHitResult CursorHit;

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//TargetData objects are replicated to the server. In our case, we just need 1 target result, so SingleTargetHit is appropriate
	//In reality what is replicated is the TargetDataHandle, so we need to create one
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	//We need to send this TargetData to the server
	FGameplayTag ApplicationTag; //For now, let's pass an empti tag
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		ApplicationTag, 
		AbilitySystemComponent->ScopedPredictionKey);


	//We still have to broadcast locally our TargetData, but first we need to check if we should broadcast (i.e. Ability is still active)

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	

}

/// <summary>
/// Callback function that will be called in response to receiving replicated target data
/// </summary>
/// <param name="DataHandle"></param>
/// <param name="ActivationTag"></param>
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	//We need to broadcast it
	//But before, we need to inform the server it's being consumed. This is because the server stores target data in a specific struct 
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	//We still have to broadcast locally our TargetData, but first we need to check if we should broadcast (i.e. Ability is still active)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}


}
