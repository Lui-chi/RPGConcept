// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	health = 1.f;
	hasTakenDamage = false;
	isDead = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::TakeDamage(float _damage)
{
	health -= _damage;

	if (health <= 0.f)
	{
		isDead = true;
	}
	else
	{
		hasTakenDamage = true;
	}
}

