// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGConceptGameMode.h"
#include "RPGConceptCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPGConceptGameMode::ARPGConceptGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
