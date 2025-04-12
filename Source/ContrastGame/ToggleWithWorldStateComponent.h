// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToggleWithWorldStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONTRASTGAME_API UToggleWithWorldStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToggleWithWorldStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	bool IsLightWorldObject = true;

	//Static Mesh Component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	//Toggle state function
	void ToggleState();

	//Light world material
	UPROPERTY(EditAnywhere)
	UMaterialInterface* VisibleMaterial;

	//Dark world material
	UPROPERTY(EditAnywhere)
	UMaterialInterface* NotVisibleMaterial;
};
