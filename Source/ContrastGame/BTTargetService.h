// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTTargetService.generated.h"

/**
 * 
 */
UCLASS()
class CONTRASTGAME_API UBTTargetService : public UBTService
{
	GENERATED_BODY()
	
private:
	class APawn* PlayerPawn;

public:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
