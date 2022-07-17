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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void TakeDamage(float _damage);


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


private:
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		float health;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		bool hasTakenDamage;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		bool isDead;
};
