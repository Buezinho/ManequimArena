// Copyright to Bue.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ManequimAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class MANEQUIMARENA_API AManequimAIController : public AAIController
{
	GENERATED_BODY()

public:
	AManequimAIController();
	
protected:

	/*UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;*/

	UPROPERTY()
	TObjectPtr <UBehaviorTreeComponent> BehaviorTreeComponent;

};
