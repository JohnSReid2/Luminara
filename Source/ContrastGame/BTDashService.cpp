// Fill out your copyright notice in the Description page of Project Settings.


#include "ContrastGameCharacter.h"
#include "BTTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIPawn.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BTDashService.h"


//on gameplay task activated
void UBTDashService::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

//on gameplay task deactivated
void UBTDashService::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
}

//tick node
void UBTDashService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//get dash enabled from the blackboard
	bool dashEnabled = OwnerComp.GetBlackboardComponent()->GetValueAsBool("DashEnabled");
	float abilityTimer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AbilityTimer");

	if (!dashEnabled && abilityTimer > 0.0f)
	{
		abilityTimer -= DeltaSeconds;
		
	}
	else if (dashEnabled)
	{
		abilityTimer = dashCooldown;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("AbilityTimer", abilityTimer);

	if (abilityTimer <= 0.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("DashEnabled", true);
	}
}
