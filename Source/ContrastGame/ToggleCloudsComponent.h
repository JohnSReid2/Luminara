// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "ToggleCloudsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONTRASTGAME_API UToggleCloudsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToggleCloudsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Volumetric clouds component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clouds")
	class UVolumetricCloudComponent* Clouds;

};
