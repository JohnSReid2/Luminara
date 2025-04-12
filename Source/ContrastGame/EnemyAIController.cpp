// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController() {
	// Create a floating pawn movement component
	MovementComponent = NULL;
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}


//on possess
void AEnemyAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	
	AEnemyAIPawn* EnemyAIPawn = Cast<AEnemyAIPawn>(InPawn);

	if (EnemyAIPawn->BehaviorTree && EnemyAIPawn->BehaviorTree->BlackboardAsset) {
		BlackboardComp->InitializeBlackboard(*(EnemyAIPawn->BehaviorTree->BlackboardAsset));

		BlackboardComp->SetValueAsFloat("FireDistance", EnemyAIPawn->FireDistance);

		BlackboardComp->SetValueAsFloat("DashDistance", EnemyAIPawn->DashDistance);

		BlackboardComp->SetValueAsFloat("DashForce", EnemyAIPawn->DashForce);

		BlackboardComp->SetValueAsFloat("AbilityCooldown", EnemyAIPawn->DashCooldown);
		
		BlackboardComp->SetValueAsFloat("MaxDistance", EnemyAIPawn->MaxDistanceFromSpawn);

		BlackboardComp->SetValueAsFloat("PatrolPlayerDistance", EnemyAIPawn->PatrolPlayerDistance);

		BlackboardComp->SetValueAsFloat("PatrolHeight", EnemyAIPawn->PatrolHeight);
		
		BehaviorTreeComp->StartTree(*EnemyAIPawn->BehaviorTree);

	}
}

//on tick
void AEnemyAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	AEnemyAIPawn* EnemyAIPawn = Cast<AEnemyAIPawn>(GetPawn());

	//EnemyAIPawn->MovementComponent->AddInputVector(FVector(1.0f, 0.0f, 0.0f));
}