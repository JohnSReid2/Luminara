// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ContrastGameGameMode.h"
#include "AIController.h"
#include "Misc/OutputDeviceNull.h"
#include "EnemyProjectile.h"
#include "EnemyAIPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyAIPawn::AEnemyAIPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Get the character movement component
	MovementComponent = GetCharacterMovement();

	//Set max speed
	MovementComponent->MaxFlySpeed = 300.0f;
	//disable gravity
	MovementComponent->GravityScale = 0.0f;
	//Set the movement mode to flying
	MovementComponent->SetMovementMode(MOVE_Flying);

	//Get the collision capsule component

	UCapsuleComponent* CollisionComponent = GetCapsuleComponent();

	CollisionComponent->OnComponentHit.AddDynamic(this, &AEnemyAIPawn::OnHit);

	USkeletalMeshComponent* MeshComponent = GetMesh();
	FRotator Rotation = MeshComponent->GetComponentRotation();
	Rotation.Yaw -= 90.0f;
	MeshComponent->SetWorldRotation(Rotation);

	//set root component to the collision component
	RootComponent = CollisionComponent;

	
}

// Called when the game starts or when spawned
void AEnemyAIPawn::BeginPlay()

{
	Super::BeginPlay();

	//Set spawn location to the current location
	SpawnLocation = GetActorLocation();
	//Log Actor Location
	UE_LOG(LogTemp, Warning, TEXT("Actor Location: %s"), *GetActorLocation().ToString());
	//Log Spawn Location
	UE_LOG(LogTemp, Warning, TEXT("Spawn Location: %s"), *SpawnLocation.ToString());
	
}

// Called every frame
void AEnemyAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//get the current Light or Dark mode from the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		//Get the skeletal mesh
		USkeletalMeshComponent* MeshComponent = GetMesh();
		//if in light world
		if (GameMode->IsLightWorld)
		{
			//if the current mesh is not the light mode mesh
			if (MeshComponent->SkeletalMesh != LightModeMesh)
			{
				//set this actors skeletal mesh to the light mode mesh
				MeshComponent->SetSkeletalMesh(LightModeMesh);

				FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				AAIController* AIController = Cast<AAIController>(GetController());
				AIController->MoveToLocation(PlayerLocation, -1.0f, false, false, false, true, 0, true);
			}

		}
		else
		{
			//if the current mesh is not the light mode mesh
			if (MeshComponent->SkeletalMesh != DarkModeMesh)
			{
				//set this actors skeletal mesh to the light mode mesh
				MeshComponent->SetSkeletalMesh(DarkModeMesh);

				FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				AAIController* AIController = Cast<AAIController>(GetController());
				AIController->MoveToLocation(PlayerLocation, -1.0f, false, false, false, true, 0, true);
			}
		}
	}
}

// On hit
void AEnemyAIPawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Log HIT
	UE_LOG(LogTemp, Warning, TEXT("HIT"));
	//if the other actor is the player
	if (OtherActor->ActorHasTag("Player"))
	{
		//Get the player movement component
		UCharacterMovementComponent* PlayerMovementComponent = OtherActor->FindComponentByClass<UCharacterMovementComponent>();
		//if the player movement component is valid
		if (PlayerMovementComponent)
		{
			//get the forward vector of this actor
			FVector ForwardVector = GetActorForwardVector();
			FVector LaunchVector = ForwardVector * 1000.0f;
			LaunchVector.Z = 200.0f;

			PlayerMovementComponent->Launch(LaunchVector);

			FOutputDeviceNull OutputDeviceNull;
			const TCHAR* CmdAndParams = TEXT("CallTakeDamage 5");
			OtherActor->CallFunctionByNameWithArguments(CmdAndParams, OutputDeviceNull, nullptr, true);
		}
	}
}

//Fire
void AEnemyAIPawn::Fire()
{


	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		//find actors of projectile class
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ProjectileClass, FoundActors);
		//for each actor
		for (AActor* Actor : FoundActors)
		{
			//if the actor is the projectile class
			if (Actor->IsA(ProjectileClass))
			{
				//log projectile already exists
				UE_LOG(LogTemp, Warning, TEXT("Projectile Already Exists"));
				return;
			}
		}


		
		//log firing
		UE_LOG(LogTemp, Warning, TEXT("Can Fire"));
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(85.0f, 0.0f, -50.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		//Draw muzzle location
		DrawDebugSphere(GetWorld(), MuzzleLocation, 10.0f, 12, FColor::Red, false, 1.0f);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AEnemyProjectile* Projectile = World->SpawnActor<AEnemyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Get the direction to the player
				FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				FVector Direction = PlayerLocation - GetActorLocation();
				Direction.Normalize();
				//Disable collision between the projectile and the enemy
				

				Projectile->FireInDirection(Direction);
			}
		}
	}
}