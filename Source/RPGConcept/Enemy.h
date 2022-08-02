// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class RPGCONCEPT_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		virtual float TakeDamage
		(
			float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			AActor* DamageCauser
		);


	UFUNCTION()
		void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimationInstance* AnimInstance;

protected:
	// Called when the game starts or when spawned
		virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, Category = "Weapon")
		class ARPGConceptCharacter* RPGPlayer;


	//GETTERS
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		float GetHealth() { return health; }

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		bool GetTakenDamage() { return hasTakenDamage; }

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		bool GetIsDead() { return isDead; }


	//SETTERS
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void SetHealth(float _health) { health = _health; }


	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void SetTakenDamage(bool toTakeDamage) { hasTakenDamage = toTakeDamage; }

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void SetIsDead(bool _isDead) { isDead = _isDead; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
		bool DoOnce;


private:
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		float health;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		bool hasTakenDamage;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		bool isDead;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
		void CheckAnimation();

	UPROPERTY()
		FTimerHandle CheckAnims;
};
