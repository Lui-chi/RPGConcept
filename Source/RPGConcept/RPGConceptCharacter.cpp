// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGConceptCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "RPGConceptData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimationInstance.h"
#include "Components/SphereComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Arrow.h"
#include "BaseInteractable.h"


//////////////////////////////////////////////////////////////////////////
// ARPGConceptCharacter

ARPGConceptCharacter::ARPGConceptCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 333.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	//Initialization


	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Fire Point"));
	FirePoint->SetupAttachment(GetCapsuleComponent());



	isOverlappingItem = false;
	isSprinting = false;
	playerHealth = 0.5f;
	playerStamina = 1.0f;
	isZoomedIn = false;

	currentLevel = 1;
	upgradePoints = 5;
	strengthValue = 1;
	dexterityValue = 1;
	intellectValue = 1;

	experiencePoints = 0.f;
	experienceToLevel = 100.f;

	attackSpeed = 1.f;
	playerDamage = 1.f;


	hasSheated = false;
	CurrentWeapon = nullptr;
	CurrentBow = nullptr;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARPGConceptCharacter::OnOverlapBegin); 
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARPGConceptCharacter::OnOverlapEnd);
}





//////////////////////////////////////////////////////////////////////////
// Input


void ARPGConceptCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARPGConceptCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARPGConceptCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ARPGConceptCharacter::PickupItem);
	PlayerInputComponent->BindAction("Equip", IE_Released , this, &ARPGConceptCharacter::PickupItem);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARPGConceptCharacter::Attack);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ARPGConceptCharacter::Roll);
	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &ARPGConceptCharacter::Parry);
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &ARPGConceptCharacter::Kick);

	PlayerInputComponent->BindAction("Sheat", IE_Pressed, this, &ARPGConceptCharacter::Sheat);
	PlayerInputComponent->BindAction("Bow", IE_Pressed, this, &ARPGConceptCharacter::Bow);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ARPGConceptCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ARPGConceptCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ARPGConceptCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ARPGConceptCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPGConceptCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPGConceptCharacter::TouchStopped);
}





//MOVEMENT FUNCTIONS


void ARPGConceptCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ARPGConceptCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}





void ARPGConceptCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ARPGConceptCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


void ARPGConceptCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGConceptCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void ARPGConceptCharacter::Sprint()
{
	isSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 666.f;
}

void ARPGConceptCharacter::StopSprinting()
{
	isSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 333.f;
}



//USE ITEMS FUNCTIONS

void ARPGConceptCharacter::OnPressed(FRPGConceptItemInfo RPGItemInfo)
{

	int32 index = Inventory.Find(RPGItemInfo);
	index = FMath::Clamp(index, 0, 24);

	if (Inventory.Contains(RPGItemInfo))
	{
		Inventory[index].CurrentStack--;
		if (RPGItemInfo.ItemType == ERPGConceptItemType::Sword)
		{
			Weapon = RPGItemInfo.ItemClass;
			if (CurrentWeapon)
			{
				hasSheated = !hasSheated;
			}
			else
			{
				CurrentWeapon = GetWorld()->SpawnActor<ABaseInteractable>(Weapon);
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("SwordSocket"));
			}

		}

		if (RPGItemInfo.ItemType == ERPGConceptItemType::Bow)
		{
			Weapon = RPGItemInfo.ItemClass;
			if (CurrentBow)
			{
				hasDrawnBow = !hasDrawnBow;
			}
			else
			{
				CurrentBow = GetWorld()->SpawnActor<ABaseInteractable>(Weapon);
				CurrentBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BowSocket"));
			}

		}


		if (RPGItemInfo.ItemType == ERPGConceptItemType::HealthItem)
		{
			Heal(RPGItemInfo.Potency);
		}

		if (RPGItemInfo.ItemType == ERPGConceptItemType::Shield)
		{
			Weapon = RPGItemInfo.ItemClass;
			CurrentShield = GetWorld()->SpawnActor<ABaseInteractable>(Weapon);
			CurrentShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));
		}

		if (Inventory[index].CurrentStack <= 0)
		{
			Inventory.RemoveAt(index);
		}


	}

}

void ARPGConceptCharacter::Heal(float _healAmount)
{
	playerHealth += _healAmount;
	if (playerHealth > 1.00f)
	{
		playerHealth = 1.00f;
	}
}



void ARPGConceptCharacter::GainExperience(float _expAmount)
{
	experiencePoints += _expAmount;
	if (experiencePoints >= experienceToLevel)
	{
		currentLevel++;
		experiencePoints = FMath::Max((experiencePoints - experienceToLevel), 0.f);
		experienceToLevel += 500.f;
		
	}
	
}



//ATACK FUNCTIONS


float ARPGConceptCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	playerHealth -= DamageToApply;
	return DamageToApply;
}

void ARPGConceptCharacter::Attack()
{
	if (CombatStance)
	{
		//Get a reference to our custom anim instance and tell it to update our character's animation
		AnimInstance = Cast<UAnimationInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->bAcceptsFirstAttackInput = true;
			AnimInstance->Attack();
		}
	}

	if (BowStance)
	{
		AnimInstance = Cast<UAnimationInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->Fire();
		}
	}
}

void ARPGConceptCharacter::Parry()
{
	if (CombatStance)
	{
		//Get a reference to our custom anim instance and tell it to update our character's animation
		AnimInstance = Cast<UAnimationInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->Parry();
		}
	}
}

void ARPGConceptCharacter::Kick()
{
	AnimInstance = Cast<UAnimationInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->Kick();
	}
}

void ARPGConceptCharacter::Roll()
{

	//Get a reference to our custom anim instance and tell it to update our character's animation
	AnimInstance = Cast<UAnimationInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (CombatStance)
		{
			EnableRotation(false);
		}
		
		AnimInstance->Roll();
	}
}


void ARPGConceptCharacter::FireArrow()
{
	if (ArrowClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = FirePoint->GetComponentTransform().GetLocation();

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Yaw += 4.5f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AArrow* Arrow = World->SpawnActor<AArrow>(ArrowClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Arrow)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Arrow->FireInDirection(LaunchDirection);
			}
		}
	}
	
}

void ARPGConceptCharacter::CheckCollisionAttack()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> HitArray;
	bool bSuccess = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		CurrentWeapon->Weapon->GetSocketLocation("WeaponStart"),
		CurrentWeapon->Weapon->GetSocketLocation("WeaponEnd"),
		10.f,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitArray,
		true,
		FLinearColor::Black,
		FLinearColor::Yellow,
		3.f);
	if (bSuccess)
	{
		for (const FHitResult Hit : HitArray)
		{
			if (Hit.GetActor()->ActorHasTag("Enemy"))
			{
				AEnemy* HitEnemy;
				HitEnemy = Cast<AEnemy>(Hit.GetActor());
				TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass(); //Dont know what does but works
				HitEnemy->TakeDamage(0.1f, FDamageEvent(DmgTypeClass), GetController(), this);
			}
			
		}
	}

}

void ARPGConceptCharacter::CheckCollisionParry()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> HitArray;
	bool bSuccess = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		CurrentShield->Weapon->GetSocketLocation("ShieldParry"),
		CurrentShield->Weapon->GetSocketLocation("ShieldParryEnd"),
		50.f,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitArray,
		true,
		FLinearColor::Black,
		FLinearColor::Yellow,
		3.f);
	if (bSuccess)
	{
		for (const FHitResult Hit : HitArray)
		{
			if (Hit.GetActor()->ActorHasTag("EnemySword"))
			{
				AEnemy* HitEnemy;
				HitEnemy = Cast<AEnemy>(Hit.GetActor());
				TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass(); //Dont know what does but works
				HitEnemy->TakeDamage(0.1f, FDamageEvent(DmgTypeClass), GetController(), this);
			}

		}
	}

}

void ARPGConceptCharacter::Sheat()
{
	if (!BowStance && CurrentWeapon)
	{
		hasSheated = !hasSheated;
		CameraShift();
		if (hasSheated)
		{
			EnableRotation(false);
		}
		else 
		{
			EnableRotation(true);
		}
	}

}

void ARPGConceptCharacter::Bow()
{
	if (!CombatStance && CurrentBow)
	{
		hasDrawnBow = !hasDrawnBow;
		CameraShift();
		if (hasDrawnBow)
		{
			EnableRotation(false);
		}
		else
		{
			EnableRotation(true);
		}
	}

}

void ARPGConceptCharacter::EnableRotation(bool Enable)
{
	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = Enable;
	characterMovement->bUseControllerDesiredRotation = !Enable;
}



//INVENTORY FUNCTIONS


void ARPGConceptCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappingActor != OtherActor && (OtherActor != CurrentWeapon) && (OtherActor != CurrentBow) && (OtherActor != CurrentShield))
	{
		OverlappingActor = OtherActor;
		if (OverlappingActor)
		{
			isOverlappingItem = true;
		}
	}

}

void ARPGConceptCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isOverlappingItem = false;
}



void ARPGConceptCharacter::PickupItem()
{
	if (isOverlappingItem)
	{
		bool isUnique = true;
		bool isFull = false;
		int32 index = 0;
		IReactToTriggerInterface* MyTrigger = Cast<IReactToTriggerInterface>(OverlappingActor);
		if (MyTrigger)
		{
			FRPGConceptItemInfo ReturnedStruct = MyTrigger->GetMyStruct();

			for (FRPGConceptItemInfo InventoryName : Inventory)
			{
				if (ReturnedStruct.ItemName == InventoryName.ItemName && index >= 0)
				{
					if (Inventory[index].CurrentStack <= Inventory[index].MaxStack)
					{
						Inventory[index].CurrentStack++;
						UE_LOG(LogTemp, Warning, TEXT("%i"), InventoryName.CurrentStack);
						isUnique = false;
					}
					else
					{
						isFull = true;
					}

				}
				index++;

			}
			if (isUnique)
			{
				Inventory.Add(ReturnedStruct);
				UE_LOG(LogTemp, Warning, TEXT("Added"));
			}
			if (!isFull)
			{
				OverlappingActor->SetActorHiddenInGame(true);
				OverlappingActor->SetActorEnableCollision(false);
				isOverlappingItem = false;
			}


		}

	}

}


void ARPGConceptCharacter::Withdrawing()
{
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
	CurrentShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponShieldSocket"));
}

void ARPGConceptCharacter::WithdrawingBow()
{
	CurrentBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponBowSocket"));
}

void ARPGConceptCharacter::Sheating()
{
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("SwordSocket"));
	CurrentShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));
}

void ARPGConceptCharacter::SheatingBow()
{
	CurrentBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BowSocket"));
}

void ARPGConceptCharacter::ChangeSword()
{

}



