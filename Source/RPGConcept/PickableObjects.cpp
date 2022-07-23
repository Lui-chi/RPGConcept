// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObjects.h"
#include "Components/CapsuleComponent.h"
#include "RPGConceptCharacter.h"
#include "Items.h"

// Sets default values
APickableObjects::APickableObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickableObjects::OnOverlapBegin);
	ItemComponent = CreateDefaultSubobject<UItems>(TEXT("ItemComponent"));
}

// Called when the game starts or when spawned
void APickableObjects::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableObjects::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


}


