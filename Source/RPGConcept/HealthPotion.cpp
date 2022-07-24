// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "RPGConceptCharacter.h"
#include "InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

AHealthPotion::AHealthPotion()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Collider);
}

void AHealthPotion::Use(ARPGConceptCharacter* Character)
{
	if (Character)
	{
		Character->Heal(HealthToHeal);
		Character->Inventory->RemoveItem(this);
	}
}
