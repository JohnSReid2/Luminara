// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyProjectile.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 900.0f;
		ProjectileMovementComponent->MaxSpeed = 900.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.05f;
		ProjectileMovementComponent->bRotationFollowsVelocity = false;
	}

	InitialLifeSpan = 3.0f;

	// Event called when component hits something.
	CollisionComponent->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate the new rotation for this frame
	FRotator NewRotation = SpinSpeed * DeltaTime;
	AddActorLocalRotation(NewRotation);


}

void AEnemyProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;

	//Spin the projectile while it moves
}

// Function that is called when the projectile hits something.
void AEnemyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//Log my name
	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *GetName());

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
			const TCHAR* CmdAndParams = TEXT("CallTakeDamage 10");
			OtherActor->CallFunctionByNameWithArguments(CmdAndParams, OutputDeviceNull, nullptr, true);
		}
	}

	Destroy();
}