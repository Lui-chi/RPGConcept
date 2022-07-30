// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGConceptData.h"
#include "RPGConceptCharacter.generated.h"



UCLASS()
class ARPGConceptCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	
	

public:
	ARPGConceptCharacter();




	// APawn interface

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	//UFUNCTION(BlueprintCallable, Category = "Items")
	//	void UseItem(class AItems* Item);
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	//class UInventoryComponent* Inventory;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		int32 ClickCounter;

	//GETTERS


	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool GetHasAttacked() { return hasAttacked; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool GetComboEnd() { return ComboEnd; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool GetComboFailed() { return ComboFailed; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool GetComboSuccess() { return ComboSuccess; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		float GetAttackSpeed() { return attackSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		float GetPlayerDamage() { return playerDamage * 0.01f; }

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetPlayerHealth() { return playerHealth; }

	UFUNCTION(BlueprintCallable, Category = "Heal")
		float GetPlayerStamina() { return playerStamina; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetPlayerLevel() { return currentLevel; }

	//SETTERS


	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetHasAttacked(bool Attacked) { hasAttacked = Attacked; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetComboEnd(bool Combo) { ComboEnd = Combo; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetComboFailed(bool Combo) { ComboFailed = Combo; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetComboSuccess(bool Combo) { ComboSuccess = Combo; }

	UFUNCTION(BlueprintCallable, Category = "Heal")
		void Heal(float _healAmount);

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetPlayerDamage(float _increaseAmount) { playerDamage = basePlayerDamage + _increaseAmount; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetPlayerAttackSpeed(float _increaseAmount) { attackSpeed = baseAttackSpeed + _increaseAmount; }



protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Sprint();

	void StopSprinting();

	void ZoomIn();

	void ZoomOut();

	void WeaponModifiers();


	UFUNCTION(BlueprintCallable, Category = "Enemy")
		virtual float TakeDamage
		(
			float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			AActor* DamageCauser
		);


	UFUNCTION(BlueprintCallable, Category = "Attack")
		void Attack();



	UFUNCTION(BlueprintCallable, Category = "Items")
		bool GetisOverlappingItem() { return isOverlappingItem; }


	UFUNCTION(BlueprintCallable, Category = "Items")
		void SetisOverlappingItem(bool Overlapping) { isOverlappingItem = Overlapping; }

	UFUNCTION(BlueprintCallable, Category = "Items")
		void PickupItem();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void GainExperience(float _expAmount);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FRPGConceptItemInfo> Inventory;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	



protected:

	//ImplementableEventsFunction

	UFUNCTION(BlueprintCallable)
		void OnPressed(FRPGConceptItemInfo RPGItemInfo);


private:


	UPROPERTY()
		TSubclassOf<ABaseInteractable> Weapon {nullptr};

	UPROPERTY()
		TSubclassOf<ABaseInteractable> Shield{ nullptr };

	UPROPERTY()
		AActor* OverlappingActor;

	UPROPERTY(EditAnywhere, Category = "Movement")
		bool isSprinting;


	UPROPERTY(EditAnywhere, Category = "Health")
		float playerHealth;


	UPROPERTY(EditAnywhere, Category = "Health")
		float playerStamina;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float playerDamage;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float basePlayerDamage;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		float baseAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Items")
		bool isOverlappingItem;

	UPROPERTY()
		bool isZoomedIn;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		float experiencePoints;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		float experienceToLevel;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		int currentLevel;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		int upgradePoints;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		int strengthValue;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		int dexterityValue;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
		int intellectValue;
	
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		float attackSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool hasAttacked;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool ComboEnd;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool ComboFailed;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool ComboSuccess;




	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		class AEnemy* Enemy;


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

