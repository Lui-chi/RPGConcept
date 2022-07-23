// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItems.h"
#include "InventoryComponent.h"
#include "RPGConceptCharacter.h"

void UFoodItems::Use(ARPGConceptCharacter* Character)
{
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Healing"));
		Character->Heal(HealthToHeal);

		if (Character->Inventory)
		{
			Character->Inventory->RemoveItem(this);
		}
	}
}
