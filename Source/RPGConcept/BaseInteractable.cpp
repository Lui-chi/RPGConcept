// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractable.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "RPGConceptData.h"


// Sets default values
ABaseInteractable::ABaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(RootComponent);

	Item = CreateDefaultSubobject<UStaticMeshComponent>("Item");
	Item->SetupAttachment(SphereCollision);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(SphereCollision);
}

// Called when the game starts or when spawned
void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();


	if (DataTable->FindRow<FRPGConceptItemInfo>(FName(ItemInfo.ItemName), "", true) != nullptr)
	{
		FRPGConceptItemInfo* FItem;
		FItem = DataTable->FindRow<FRPGConceptItemInfo>(FName(ItemInfo.ItemName), "", true);
		ItemInfo = *FItem;
		UE_LOG(LogTemp, Warning, TEXT("%i"), ItemInfo.SellValue);
	}
}

// Called every frame
void ABaseInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FRPGConceptItemInfo ABaseInteractable::GetMyStruct()
{
	return ItemInfo;
}


