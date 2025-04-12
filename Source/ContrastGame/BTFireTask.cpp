// Fill out your copyright notice in the Description page of Project Settings.


#include "BTFireTask.h"
#include "ContrastGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIPawn.h"
#include "EnemyAIController.h"
#include "kismet/GameplayStatics.h"

EBTNodeResult::Type UBTFireTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	//get CanFire from the blackboard
	bool CanFire = OwnerComp.GetBlackboardComponent()->GetValueAsBool("CanFire");

	if (CanFire)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanFire", false);
		AEnemyAIPawn* OwnerPawn = Cast<AEnemyAIPawn>(AIController->GetPawn());

		if (OwnerPawn)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanFire", false);
			//Fire the weapon
			OwnerPawn->Fire();
			//log firing
			
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTFireTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
}