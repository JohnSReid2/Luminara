// Fill out your copyright notice in the Description page of Project Settings.

#include "ContrastGameCharacter.h"
#include "BTTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIPawn.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void UBTTargetService::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

void UBTTargetService::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
}

void UBTTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UWorld* World = OwnerComp.GetWorld();

	//Get the player pawn
	if (PlayerPawn == nullptr)
	{
		PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	}

	if (PlayerPawn != nullptr) {

		AEnemyAIController* EnemyAI = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		APawn* OwnerPawn = EnemyAI->GetPawn();

		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		
		FVector TargetDirection = PlayerLocation - OwnerPawn->GetActorLocation();

		float Distance = TargetDirection.Size();
		
		//log the player location
		EnemyAI->BlackboardComp->SetValueAsVector("PlayerLocation", PlayerLocation);
		EnemyAI->BlackboardComp->SetValueAsVector("TargetDirection", TargetDirection);
		EnemyAI->BlackboardComp->SetValueAsFloat("Distance", Distance);
	}

}
