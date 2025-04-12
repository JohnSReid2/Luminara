// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ContrastGameGameMode.generated.h"

UCLASS(minimalapi)
class AContrastGameGameMode : public AGameModeBase
{
	GENERATED_BODY()



public:
	AContrastGameGameMode();

	// IsLightWorld is a boolean that determines if the player is in the light world or the dark world
	UPROPERTY(BlueprintReadWrite, Category = "Contrast")
	bool IsLightWorld = true;

protected:
	virtual void Tick(float DeltaTime) override;
};



