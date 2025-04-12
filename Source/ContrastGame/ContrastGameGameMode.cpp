// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContrastGameGameMode.h"
#include "ContrastGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AContrastGameGameMode::AContrastGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	//can tick
	PrimaryActorTick.bCanEverTick = true;

	IsLightWorld = true;
}

// Called every frame
void AContrastGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Log warning if the player is in the light world
}