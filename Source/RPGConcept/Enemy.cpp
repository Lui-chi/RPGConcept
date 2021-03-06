// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "RPGConceptCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/CapsuleComponent.h"
#include "AnimationInstance.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	health = 1.f;
	hasTakenDamage = false;
	isDead = false;
	DoOnce = true;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	RPGPlayer = Cast<ARPGConceptCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	GetWorldTimerManager().SetTimer(CheckAnims, this, &AEnemy::CheckAnimation, 0.2f, true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("ziiiii1"));
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	health -= DamageToApply;

	if (health <= 0.f)
	{
		isDead = true;
	}
	else
	{
		hasTakenDamage = true;
	}
	return DamageToApply;
}

void AEnemy::Attack()
{
	AnimInstance = Cast<UAnimationInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bAcceptsFirstAttackInput = true;
		AnimInstance->Attack();
	}

}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	/*AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	GetWorldTimerManager().SetTimer(CheckAnims, this, &AEnemy::CheckAnimation, 0.2f, true);
	if (Weapon && RPGPlayer)
	{
		if (RPGPlayer->GetHasAttacked() && DoOnce)
		{	
			FVector ShotDirection;
			FHitResult Hit;
			AActor* HitActor = Hit.GetActor();
			AController* OwnerController = GetController();
			FPointDamageEvent DamageEvent(RPGPlayer->GetPlayerDamage(), Hit, ShotDirection, nullptr);

			TakeDamage(RPGPlayer->GetPlayerDamage(), DamageEvent, OwnerController, OtherActor);	
			UE_LOG(LogTemp, Warning, TEXT("111111"));
			DoOnce = false;
		}

			

	}*/
}

void AEnemy::CheckAnimation()
{

		DoOnce = true;
		GetWorldTimerManager().ClearTimer(CheckAnims);
}

