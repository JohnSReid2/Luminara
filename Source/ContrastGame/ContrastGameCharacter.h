// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ContrastGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AContrastGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Toggle Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleAction;

	//Dash action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	//Attack action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	
public:
	AContrastGameCharacter();

protected:
	virtual void BeginPlay();

	virtual void Landed(const FHitResult& Hit) override;

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//Dash force value
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float DashForce = 1000.0f;

	//Dash cooldown value
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float DashCooldown = 1.0f;

	//Toggle Cooldown value
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float ToggleCooldown = 1.0f;

	//Toggle Cooldown value
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float AttackCooldown = 5.0f;

	//Double Jump force
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float DoubleJumpForce = 1000.0f;

	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bFirstJump = true;

	//second jump   
	UPROPERTY(EditAnywhere, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bSecondJump = true;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called for toggling input
	void Toggle(const FInputActionValue& Value);

	// Called for dashing input
	void Dash(const FInputActionValue& Value);

	// Double jump function
	void DoubleJump();

	// Attack function
	void Attack();

	//Tick function
	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

	//Dash cooldown timer
	float DashCooldownTimer = 0.0f;

	//Toggle cooldown timer
	float ToggleCooldownTimer = 0.0f;

	//Toggle cooldown timer
	float AttackCooldownTimer = 0.0f;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

