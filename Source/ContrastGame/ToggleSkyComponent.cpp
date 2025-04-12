// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleSkyComponent.h"
#include "ContrastGameGameMode.h"

// Sets default values for this component's properties
UToggleSkyComponent::UToggleSkyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UToggleSkyComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the sky atmosphere component
	SkyAtmosphere = GetOwner()->FindComponentByClass<USkyAtmosphereComponent>();
	if (!SkyAtmosphere)
	{
		UE_LOG(LogTemp, Error, TEXT("Sky Atmosphere component not found"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sky Atmosphere component found"));
	}
	
}


// Called every frame
void UToggleSkyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get the current Light or Dark mode from the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		//if in light world
		if (GameMode->IsLightWorld)
		{
			//set the sky atmosphere rayleigh scattering to light
			SkyAtmosphere->SetRayleighScattering(FLinearColor(0.6f, 0.2096f, 1.0f, 1.0f));

		}
		else
		{
			//set the sky atmosphere rayleigh scattering to dark
			SkyAtmosphere->SetRayleighScattering(FLinearColor(0.0055f, 0.00f, 0.004f, 1.0f));
		}
	}
}

