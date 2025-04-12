// Fill out your copyright notice in the Description page of Project Settings.


#include "BTFireService.h"
#include "ContrastGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIPawn.h"
#include "EnemyAIController.h"
#include "ContrastGameGameMode.h"
#include "Kismet/GameplayStatics.h"


void UBTFireService::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

void UBTFireService::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
}

void UBTFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UWorld* World = OwnerComp.GetWorld();

	//Get the player pawn
	if (PlayerPawn == nullptr)
	{
		PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	}

	//Check world state (lightmode)
	//get the current Light or Dark mode from the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		//if in light world
		if (!GameMode->IsLightWorld)
		{
			return;
		}
	}
	else {
		return;
	}

	if (PlayerPawn != nullptr) {

		float FireDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("FireDistance");


		AEnemyAIController* EnemyAI = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		APawn* OwnerPawn = EnemyAI->GetPawn();

		FVector PlayerLocation = PlayerPawn->GetActorLocation();

		FVector TargetDirection = PlayerLocation - OwnerPawn->GetActorLocation();

		float Distance = TargetDirection.Size();

		if (Distance <= FireDistance) {
			//Set Can fire to true in blackboard
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanFire", true);
			//log set fire to true
			UE_LOG(LogTemp, Warning, TEXT("Can Fire"));
		}
	}

}