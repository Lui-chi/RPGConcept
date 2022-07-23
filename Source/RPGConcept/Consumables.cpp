// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumables.h"
#include "FoodItems.h"
#include "PickableObjects.h"

AConsumables::AConsumables()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

void AConsumables::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
