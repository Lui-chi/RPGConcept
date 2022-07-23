// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Weapon.h"
#include "EquipableItems.generated.h"

/**
 * 
 */
UCLASS()
class RPGCONCEPT_API UEquipableItems : public UItems
{
	GENERATED_BODY()

		UEquipableItems();

protected:
	virtual void Use(class ARPGConceptCharacter* Character) override;

	UPROPERTY(EditAnywhere, Instanced)
		AWeapon* Weapon;

};
