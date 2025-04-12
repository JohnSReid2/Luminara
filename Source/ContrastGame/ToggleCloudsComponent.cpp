// Fill out your copyright notice in the Description page of Project Settings.

#include "ContrastGameGameMode.h"
#include "ToggleCloudsComponent.h"

// Sets default values for this component's properties
UToggleCloudsComponent::UToggleCloudsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UToggleCloudsComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the sky atmosphere component
	Clouds = GetOwner()->FindComponentByClass<UVolumetricCloudComponent>();
	if (!Clouds)
	{
		UE_LOG(LogTemp, Error, TEXT("Sky Atmosphere component not found"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sky Atmosphere component found"));
	}
	
}


// Called every frame
void UToggleCloudsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get the current Light or Dark mode from the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		//if in light world
		if (GameMode->IsLightWorld)
		{
			//set clouds to not visible
			Clouds->SetVisibility(false);

		}
		else
		{
			//Set clouds to visible
			Clouds->SetVisibility(true);
		}
	}
}

