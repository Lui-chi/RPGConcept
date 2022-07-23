// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableObjects.h"
#include "Consumables.generated.h"


/**
 * 
 */
UCLASS()
class RPGCONCEPT_API AConsumables : public APickableObjects
{
	GENERATED_BODY()
	
public:
	AConsumables();

protected:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UFoodItems* FoodItem;


	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
