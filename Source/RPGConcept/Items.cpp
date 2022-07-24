// Fill out your copyright notice in the Description page of Project Settings.


#include "Items.h"
#include "Components/CapsuleComponent.h"

AItems::AItems()
{
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}
