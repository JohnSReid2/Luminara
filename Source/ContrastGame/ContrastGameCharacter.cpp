// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContrastGameCharacter.h"
#include "ContrastGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "ContrastGameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ToggleWithWorldStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIPawn.h"
#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AContrastGameCharacter

AContrastGameCharacter::AContrastGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AContrastGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


	//toggle world state in the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		GameMode->IsLightWorld = true;
	}

	//Find all actors in the world and store them in an array
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	//Iterate through the array of actors and call the ToggleState function on each actor
	for (AActor* Actor : FoundActors)
	{
		if (UToggleWithWorldStateComponent* ToggleComponent = Actor->FindComponentByClass<UToggleWithWorldStateComponent>())
		{
			ToggleComponent->ToggleState();
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AContrastGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AContrastGameCharacter::DoubleJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AContrastGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AContrastGameCharacter::Look);

		// Toggling
		EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Triggered, this, &AContrastGameCharacter::Toggle);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AContrastGameCharacter::Dash);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AContrastGameCharacter::Attack);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// double jump function
void AContrastGameCharacter::DoubleJump()
{
	bool inLightWorld = true;
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		inLightWorld = GameMode->IsLightWorld;
	}

	if (bFirstJump) {
		bFirstJump = false;
		Jump();
	}
	else if (bSecondJump && inLightWorld){
		LaunchCharacter(FVector(0, 0, DoubleJumpForce), false, true);
		bSecondJump = false;
	}

	
}

void AContrastGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bFirstJump = true;
	bSecondJump = true;
}


void AContrastGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AContrastGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Toggle function
void AContrastGameCharacter::Toggle(const FInputActionValue& Value)
{
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());


	for (AActor* Actor : OverlappingActors)
	{
		//Log name of actor
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Actor Name: %s"), *Actor->GetName());
		if (UToggleWithWorldStateComponent* ToggleComponent = Actor->FindComponentByClass<UToggleWithWorldStateComponent>())
		{
			return;
		}
	}

	//if the toggle cooldown timer is greater than 0, return
	if (ToggleCooldownTimer > 0.0f)
	{
		return;
	}


	//toggle world state in the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		GameMode->IsLightWorld = !GameMode->IsLightWorld;
		UFunction* func = this->GetClass()->FindFunctionByName(FName("CallPlayToggleAnimation"));
		if (func)
		{
			this->ProcessEvent(func, nullptr);
		}
		ToggleCooldownTimer = ToggleCooldown;
	}

	//Find all actors in the world and store them in an array
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	//Iterate through the array of actors and call the ToggleState function on each actor
	for (AActor* Actor : FoundActors)
	{
		if (UToggleWithWorldStateComponent* ToggleComponent = Actor->FindComponentByClass<UToggleWithWorldStateComponent>())
		{
			ToggleComponent->ToggleState();
		}
	}

}

// Dash function
void AContrastGameCharacter::Dash(const FInputActionValue& Value)
{

	//if we are in the dark world
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		if (!GameMode->IsLightWorld && DashCooldownTimer <= 0.0f)
		{
			const FVector Dash = this->GetCharacterMovement()->GetLastInputVector();
			//get forward vector
			FVector DashDirection = GetActorForwardVector();

			LaunchCharacter(Dash * DashForce, true, true);
			//LaunchCharacter(DashDirection * DashForce, true, true);

			UFunction* func = this->GetClass()->FindFunctionByName(FName("CallPlayDashAnimation"));
			if (func)
			{
				this->ProcessEvent(func, nullptr);
			}

			DashCooldownTimer = DashCooldown;
		}
	}
}

// Attack function
void AContrastGameCharacter::Attack() {
	//log attack pressed 
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Attack Pressed"));

	//if the attack cooldown timer is greater than 0, return
	if (AttackCooldownTimer > 0.0f)
	{
		return;
	}
	else {
		AttackCooldownTimer = AttackCooldown;
	}


	UFunction* func = this->GetClass()->FindFunctionByName(FName("CallAttack"));
	if (func)
	{
		this->ProcessEvent(func, nullptr);
	}


	//Get all actors within a radius of 600 units around the player
	TArray<AActor*> TargetActors;

	//Get all actors in world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAIPawn::StaticClass(), TargetActors);

	//Iterate through the array of actors
	for (AActor* Actor : TargetActors)
	{
		if (AEnemyAIPawn* Enemy = Cast<AEnemyAIPawn>(Actor))
		{
			//Get the distance between the player and the enemy
			float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
			//if the distance is less than 600 units, launch the enemy
			if (Distance < 800.0f)
			{
				//Get the direction from the player to the enemy
				FVector LaunchDirection = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();

				//Get the AI controller of the enemy
				AEnemyAIController* AIController = Cast<AEnemyAIController>(Enemy->GetController());


				//Get the movement component of the enemy
				UCharacterMovementComponent* MovementComponent = Enemy->FindComponentByClass<UCharacterMovementComponent>();
				

				//Get the blackboard component of the enemy
				UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

				// Set IsDashing to true
				BlackboardComp->SetValueAsBool("IsDashing", true);

				// Stop AI movement temporarily
				AIController->GetPathFollowingComponent()->PauseMove();


				MovementComponent->Launch(LaunchDirection * 3000);
				// Set a timer to re-enable movement and reset IsDashing
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([AIController, BlackboardComp, MovementComponent, Enemy]()
					{
						AIController->GetPathFollowingComponent()->ResumeMove();
						//Log "DASH COMPLETE"
						BlackboardComp->SetValueAsBool("IsDashing", false);
						MovementComponent->StopMovementImmediately();
						MovementComponent->Velocity = FVector::ZeroVector;
						MovementComponent->SetMovementMode(MOVE_Flying);
						MovementComponent->Activate();

						FVector AdjustedLocation = Enemy->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f); // Small upward adjustment
						Enemy->SetActorLocation(AdjustedLocation, true);
						//log enemy launched
						UE_LOG(LogTemplateCharacter, Warning, TEXT("Enemy Launched"));
					});
				Enemy->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false); // Adjust 0.5f for dash duration
			}
		}
	}
	
}


//Tick function
void AContrastGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Decrement the dash cooldown timer
	if (DashCooldownTimer > 0.0f)
	{
		DashCooldownTimer -= DeltaTime;
	}

	//Decrement the toggle cooldown timer
	if (ToggleCooldownTimer > 0.0f)
	{
		ToggleCooldownTimer -= DeltaTime;
	}

	//Decrement the attack cooldown timer
	if (AttackCooldownTimer > 0.0f)
	{
		AttackCooldownTimer -= DeltaTime;
	}
}