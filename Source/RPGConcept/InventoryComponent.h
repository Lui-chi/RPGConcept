// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGCONCEPT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	bool AddItem(class AItems* Item);
	bool RemoveItem(class AItems* Item);

	UPROPERTY(EditAnywhere, Instanced, Category = "Inventory")
		TArray<class AItems*> DefaultItems;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		int32 Capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(EditAnywhere, Instanced,  BlueprintReadOnly, Category = "Inventory")
		TArray<class AItems*> Items;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
