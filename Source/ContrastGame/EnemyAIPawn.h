// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyAIPawn.generated.h"

UCLASS()
class CONTRASTGAME_API AEnemyAIPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyAIPawn();

	// A movement component for a floating enemy
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UCharacterMovementComponent* MovementComponent;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//on collision
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Fire in direction
	UFUNCTION()
	void Fire();

	// Projectile class to spawn.
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AEnemyProjectile> ProjectileClass;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;


	//Light mode model
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMesh* LightModeMesh;

	//Dark mode model
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMesh* DarkModeMesh;

	//Spawn location of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector SpawnLocation;


	//AI Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float FireDistance = 1300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DashDistance = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DashForce = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DashCooldown = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxDistanceFromSpawn = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PatrolPlayerDistance = 8000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PatrolHeight = 300;
};
