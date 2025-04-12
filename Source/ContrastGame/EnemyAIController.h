// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CONTRASTGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	class UCharacterMovementComponent* MovementComponent;
	class UBehaviorTreeComponent* BehaviorTreeComp;
	class UBlackboardComponent* BlackboardComp;

public:
	
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;
};
