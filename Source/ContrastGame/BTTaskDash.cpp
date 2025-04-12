#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "ContrastGameGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTaskDash.h"

EBTNodeResult::Type UBTTaskDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

        if (BlackboardComp->HasValidAsset())
        {
            FVector PlayerLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
            float Distance = BlackboardComp->GetValueAsFloat("Distance");
			bool MovingToSpawn = BlackboardComp->GetValueAsBool("MovingToSpawn");


			float DashDistance = BlackboardComp->GetValueAsFloat("DashDistance");
			float DashForce = BlackboardComp->GetValueAsFloat("DashForce");
			float AbilityCooldown = BlackboardComp->GetValueAsFloat("AbilityCooldown");


			if (Distance > DashDistance || MovingToSpawn)
			{
				return EBTNodeResult::Failed;
			}


            AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>();

			if (!GameMode)
            {
				return EBTNodeResult::Failed;
            }

            //Dark World Dash
            if (!GameMode->IsLightWorld)
            {
                // Get dash enabled from the blackboard
                bool DashEnabled = BlackboardComp->GetValueAsBool("DashEnabled");
				// Get distance to player from the blackboard

                if (DashEnabled)
                {
                    // Set IsDashing to true
                    BlackboardComp->SetValueAsBool("IsDashing", true);

                    // Get AIController and Pawn
                    APawn* OwnerPawn = AIController->GetPawn();
                    UCharacterMovementComponent* MovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();

                    if (OwnerPawn && AIController && MovementComponent)
                    {
                        // Stop AI movement temporarily
                        AIController->GetPathFollowingComponent()->PauseMove();

                        FVector Direction = PlayerLocation - OwnerPawn->GetActorLocation();
                        Direction.Normalize();

                        // Launch the pawn in the specified direction
                        MovementComponent->Launch(Direction * DashForce);


                        // Set a timer to re-enable movement and reset IsDashing
                        FTimerHandle TimerHandle;
                        FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([AIController, BlackboardComp, MovementComponent, OwnerPawn]()
                            {
                                AIController->GetPathFollowingComponent()->ResumeMove();
								//Log "DASH COMPLETE"
                                BlackboardComp->SetValueAsBool("IsDashing", false);  
								MovementComponent->StopMovementImmediately();
                                MovementComponent->Velocity = FVector::ZeroVector;
                                MovementComponent->SetMovementMode(MOVE_Flying);
								MovementComponent->Activate();

                                FVector AdjustedLocation = OwnerPawn->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f); // Small upward adjustment
                                OwnerPawn->SetActorLocation(AdjustedLocation, true);
                            });
                        OwnerPawn->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false); // Adjust 0.5f for dash duration



                        // Update other blackboard values
                        BlackboardComp->SetValueAsBool("DashEnabled", false);
                        BlackboardComp->SetValueAsFloat("AbilityTimer", AbilityCooldown);

                        return EBTNodeResult::Succeeded;
                    }
                    else
                    {
                        BlackboardComp->SetValueAsBool("IsDashing", false); // Reset if failed
						AIController->GetPathFollowingComponent()->ResumeMove();
                        return EBTNodeResult::Succeeded;
                    }
                }
            }
			//Light World
            else {
                //Resuming movement
                AIController->GetPathFollowingComponent()->ResumeMove();
                BlackboardComp->SetValueAsBool("IsDashing", false);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    return EBTNodeResult::Failed;
}

void UBTTaskDash::OnGameplayTaskActivated(UGameplayTask& Task)
{
}