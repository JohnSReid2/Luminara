// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleWithWorldStateComponent.h"
#include "ContrastGameGameMode.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UToggleWithWorldStateComponent::UToggleWithWorldStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UToggleWithWorldStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//get the static mesh component
	StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	StaticMeshComponent->SetGenerateOverlapEvents(true);
}


// Called every frame
void UToggleWithWorldStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UToggleWithWorldStateComponent::ToggleState()
{
	//get the current Light or Dark mode from the game mode
	if (AContrastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AContrastGameGameMode>())
	{
		//if in light world
		if (GameMode->IsLightWorld && IsLightWorldObject)
		{
			//Set material to visible material
			StaticMeshComponent->SetMaterial(0, VisibleMaterial);
			

			//Set collision response to pawn to block
			StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}
		else if (!GameMode->IsLightWorld && !IsLightWorldObject)
		{
			//Set material to visible material
			StaticMeshComponent->SetMaterial(0, VisibleMaterial);

			//Set collision response to pawn to block
			StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}
		else {
			//Set material to visible material
			StaticMeshComponent->SetMaterial(0, NotVisibleMaterial);

			//Set collision response to pawn to overlap
			StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


		}
	}
}

