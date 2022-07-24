// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "RPGConceptCharacter.h"
#include "InventoryComponent.h"


AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

	levelReq = 1;
	baseDamage = 10.f;
	baseSpeed = 1.0;
	weaponType = EWeaponType::E_Default;
}

void AWeapon::Use(ARPGConceptCharacter* Character)
{
	if (Character->CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Si entra"));
		Character->Inventory->AddItem(Character->CurrentWeapon);
		Character->CurrentWeapon->SetActorHiddenInGame(true);
		Character->CurrentWeapon->SetActorEnableCollision(false);
	}
	if (levelReq <= Character->GetPlayerLevel())
	{
		Character->EquipWeapon(this, WeaponClass);
		Character->Inventory->RemoveItem(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("dont meet requirements"));
	}

}


void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
