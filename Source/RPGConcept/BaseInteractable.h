// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGConceptData.h"
#include "BaseInteractable.generated.h"



UCLASS()
class RPGCONCEPT_API ABaseInteractable : public AActor, public IReactToTriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractable();

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Item;

	

	UPROPERTY(EditAnywhere)
		struct FRPGConceptItemInfo ItemInfo;

	UPROPERTY(EditAnywhere)
		class UDataTable* DataTable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FRPGConceptItemInfo GetMyStruct() override;

};
