// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Default	UMETA(DisplayName = "DEFAULT"),
	E_Sword		UMETA(DisplayName = "SWORD"),
	E_Dagger	UMETA(DisplayName = "DAGGER"),
	E_Axe		UMETA(DisplayName = "AXE")
};

UCLASS()
class RPGCONCEPT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		int levelReq;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		int baseDamage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		int baseSpeed;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		EWeaponType weaponType;
private:

	//UPROPERTY(VisibleAnywhere)
	//	class USceneComponent* RootComp;
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Collider;



	UPROPERTY()
		class ARPGConceptCharacter* RPGPlayer;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
