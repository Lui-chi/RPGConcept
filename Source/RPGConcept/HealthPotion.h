// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class RPGCONCEPT_API AHealthPotion : public AItems
{
	GENERATED_BODY()
	
public:
	AHealthPotion();

protected:

	virtual void Use(class ARPGConceptCharacter* Character) override;

	UPROPERTY(EditAnywhere)
		float HealthToHeal;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;
};
