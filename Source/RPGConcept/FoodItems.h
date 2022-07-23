// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "FoodItems.generated.h"

/**
 * 
 */
UCLASS()
class RPGCONCEPT_API UFoodItems : public UItems
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float HealthToHeal;


protected:
	virtual void Use(class ARPGConceptCharacter* Character) override;

};
