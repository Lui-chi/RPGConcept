// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGConceptCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Items.h"
#include "InventoryComponent.h"
#include "Weapon.h"

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

	isOverlappingItem = false;
	isSprinting = false;
	playerHealth = 1.0f;
	playerStamina = 1.0f;
	isZoomedIn = false;

	currentLevel = 1;
	upgradePoints = 5;
	strengthValue = 1;
	dexterityValue = 1;
	intellectValue = 1;

	experiencePoints = 0.f;
	experienceToLevel = 100.f;

	hasAttacked = false;
	attackSpeed = 1.f;
	playerDamage = 1.f;

	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARPGConceptCharacter::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARPGConceptCharacter::OnOverlapBegin); 
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARPGConceptCharacter::OnOverlapEnd);
}

void ARPGConceptCharacter::UseItem(AItems* Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); //Blueprint Event
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGConceptCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARPGConceptCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARPGConceptCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ARPGConceptCharacter::PickupItem);
	PlayerInputComponent->BindAction("Equip", IE_Released , this, &ARPGConceptCharacter::PickupItem);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARPGConceptCharacter::Attack);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ARPGConceptCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ARPGConceptCharacter::ZoomOut);

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

void ARPGConceptCharacter::Heal(float _healAmount)
{
	playerHealth += _healAmount;
	if (playerHealth > 1.00f)
	{
		playerHealth = 1.00f;
	}
}

void ARPGConceptCharacter::PickupItem()
{
	if (isOverlappingItem)
	{
		Inventory->AddItem(Cast<AItems>(OverlappingActor));
		OverlappingActor->SetActorHiddenInGame(true);
		OverlappingActor->SetActorEnableCollision(false);
		isOverlappingItem = false;
	}

}

void ARPGConceptCharacter::ZoomIn()
{
	if (auto thirdPersonCamera = GetCameraBoom())
	{
		thirdPersonCamera->TargetArmLength = 150.f;
		thirdPersonCamera->TargetOffset = FVector(0.f, 0.f, 70.f);
		if (auto characterMovement = GetCharacterMovement())
		{
			characterMovement->MaxWalkSpeed = 200.f;
		}
		isZoomedIn = true;
	}	

}

void ARPGConceptCharacter::ZoomOut()
{
	if (auto thirdPersonCamera = GetCameraBoom())
	{
		thirdPersonCamera->TargetArmLength = 300.f;
		thirdPersonCamera->TargetOffset = FVector(0.f, 0.f, 0.f);
		if (auto characterMovement = GetCharacterMovement())
		{
			characterMovement->MaxWalkSpeed = 333.f;
		}
		isZoomedIn = false;
	}
}

float ARPGConceptCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	playerHealth -= DamageToApply;
	return DamageToApply;
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
	UE_LOG(LogTemp, Warning, TEXT("exp: %f  ;  level: %i"), experiencePoints, currentLevel);
}

void ARPGConceptCharacter::Attack()
{
	hasAttacked = true;
}

void ARPGConceptCharacter::WeaponModifiers()
{
}

void ARPGConceptCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void ARPGConceptCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappingActor != OtherActor)
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

void ARPGConceptCharacter::EquipWeapon(AWeapon* Weapon, TSubclassOf<AWeapon> WeaponClass)
{
	CurrentWeapon = Weapon;
	attackSpeed += CurrentWeapon->baseSpeed;
	playerDamage += CurrentWeapon->baseDamage;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
}

