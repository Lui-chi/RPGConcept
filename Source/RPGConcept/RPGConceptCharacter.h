// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGConceptCharacter.generated.h"

UCLASS(config=Game)
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

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Sprint();

	void StopSprinting();

	void ZoomIn();

	void ZoomOut();




	//GETTERS


	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool GetHasAttacked() { return hasAttacked; }


	UFUNCTION(BlueprintCallable, Category = "Heal")
		float GetPlayerHealth() { return playerHealth; }

	UFUNCTION(BlueprintCallable, Category = "Heal")
		float GetPlayerStamina() { return playerStamina; }
	//SETTERS


	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetHasAttacked(bool Attacked) { hasAttacked = Attacked; }




	UFUNCTION(BlueprintCallable, Category = "Attack")
		void Attack();



	UFUNCTION(BlueprintCallable, Category = "Heal")
		void Heal(float _healAmount);


	




	UFUNCTION(BlueprintCallable, Category = "Heal")
		void TakeDamage(float _damageAmount);

	UFUNCTION(BlueprintCallable, Category = "Items")
		bool GetisOverlappingItem() { return isOverlappingItem; }


	UFUNCTION(BlueprintCallable, Category = "Items")
		void SetisOverlappingItem(bool Overlapping) { isOverlappingItem = Overlapping; }

	UFUNCTION(BlueprintCallable, Category = "Items")
		void EquipItem();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void GainExperience(float _expAmount);

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
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface




private:
	UPROPERTY(EditAnywhere, Category = "Movement")
		bool isSprinting;


	UPROPERTY(EditAnywhere, Category = "Health")
		float playerHealth;


	UPROPERTY(EditAnywhere, Category = "Health")
		float playerStamina;



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
	

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool hasAttacked;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

