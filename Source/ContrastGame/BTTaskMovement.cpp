// Fill out your copyright notice in the Description page of Project Settings.

#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "EnemyAIPawn.h"
#include "ContrastGameGameMode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTaskMovement.h"

EBTNodeResult::Type UBTTaskMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	

	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

		if (BlackboardComp->HasValidAsset()){


			FVector PlayerLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
			FVector TargetDirection = BlackboardComp->GetValueAsVector("TargetDirection");
			float Distance = BlackboardComp->GetValueAsFloat("Distance");
			float MaxDistance = BlackboardComp->GetValueAsFloat("MaxDistance");
			float PatrolPlayerDistance = BlackboardComp->GetValueAsFloat("PatrolPlayerDistance");
			float PatrolHeight = BlackboardComp->GetValueAsFloat("PatrolHeight");

			//Get spawn location from pawn
			AEnemyAIPawn* EnemyAIPawn = Cast<AEnemyAIPawn>(AIController->GetPawn());
			FVector SpawnLocation = EnemyAIPawn->SpawnLocation;
			
			//Get the distance from the spawn location
			float DistanceFromSpawn = FVector::Dist(SpawnLocation, EnemyAIPawn->GetActorLocation());


			//get bool MovingToSpawn from blackboard
			bool MovingToSpawn = BlackboardComp->GetValueAsBool("MovingToSpawn");

			if (DistanceFromSpawn < 500 && MovingToSpawn) {
				//set MovingToSpawn to false
				BlackboardComp->SetValueAsBool("MovingToSpawn", false);
				//stop moving
				AIController->StopMovement();
				return EBTNodeResult::Succeeded;
			}

			if (DistanceFromSpawn > MaxDistance || MovingToSpawn)
			{
				//move to spawn location
				AIController->MoveToLocation(SpawnLocation, -1.0f, false, false, false, true, 0, true);
				//set MovingToSpawn to true
				BlackboardComp->SetValueAsBool("MovingToSpawn", true);
				return EBTNodeResult::Succeeded;
			}

			if (Distance > (MaxDistance - 100) && !MovingToSpawn)
			{
				return EBTNodeResult::Succeeded;
			}

			//Rotate to face the player in all worlds
			AIController->GetPawn()->SetActorRotation(TargetDirection.Rotation());

			FVector HorizontalDirection = FVector(TargetDirection.X, TargetDirection.Y, 0.0f);
			float HorizontalDistance = HorizontalDirection.Size();

			AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>();
			if (!GameMode)
			{
				return EBTNodeResult::Failed;
			}

			if (BlackboardComp->GetValueAsBool("IsDashing") == true)
			{
				return EBTNodeResult::Failed;
			}

			//Light world movement
			if (GameMode->IsLightWorld)
			{

				if (HorizontalDistance >= PatrolPlayerDistance)
				{
					PlayerLocation.Z += PatrolHeight;
					AIController->MoveToLocation(PlayerLocation, -1.0f, false, false, false, true, 0, true);
				}
				else
				{
					//Stop moving
					AIController->StopMovement();
				}
			}

			//Dark world movement
			else if (!GameMode->IsLightWorld)
			{
				//Fly to the player
				AIController->MoveToLocation(PlayerLocation, -1.0f, false, false, false, true, 0, true);
			}

			
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTaskMovement::OnGameplayTaskActivated(UGameplayTask& Task)
{
}