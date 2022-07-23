// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "RPGConceptCharacter.h"


AWeapon::AWeapon()
{


	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComp);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);

	levelReq = 1;
	baseDamage = 10.f;
	baseSpeed = 1.0;
	weaponType = EWeaponType::E_Default;
}


void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	RPGPlayer = Cast<ARPGConceptCharacter>(OtherActor);
	if (RPGPlayer)
	{
		Collider->AttachToComponent(RPGPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		Mesh->AttachToComponent(RPGPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
	}
}
