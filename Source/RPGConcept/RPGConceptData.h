#pragma once

#include "Engine/Texture2D.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RPGConceptData.generated.h"


UENUM(BlueprintType)
enum class ERPGConceptItemNames : uint8
{
	None UMETA(DisplayName = "None"),
	HealthPotion UMETA(DisplayName = "HealthPotion"),
	ManaPotion UMETA(DisplayName = "ManaPotion"),
	IronSword UMETA(DisplayName = "IronSword"),
	IronShield UMETA(DisplayName = "IronShield")
};

UENUM(BlueprintType)
enum class ERPGConceptItemType : uint8
{
	None UMETA(DisplayName = "None"),
	HealthItem UMETA(DisplayName = "HealthItem"),
	Sword UMETA(DisplayName = "Sword"),
	Shield UMETA(DisplayName = "Shield"),
	KeyItem UMETA(DisplayName = "KeyItem"),
	Resources UMETA(DisplayName = "Resources")
};

USTRUCT(BlueprintType)
struct RPGCONCEPT_API FRPGConceptItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<class ABaseInteractable>  ItemClass;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FName ItemName;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			UTexture2D* ItemImage;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int CurrentStack;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int MaxStack;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int ItemIndex;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			ERPGConceptItemType ItemType;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float Potency;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int Price;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int SellValue;

		FRPGConceptItemInfo()
		{
			ItemClass = nullptr;
			ItemName = FName("None");
			ItemImage = nullptr;
			CurrentStack = 0;
			MaxStack = 0;
			ItemIndex = 0;
			ItemType = ERPGConceptItemType::None;
			Potency = 0;
			Price = 0;
			SellValue = 0;


	}
		
	FORCEINLINE bool operator==(const FRPGConceptItemInfo& Other) const
	{
		return ItemIndex == Other.ItemIndex;
	}

};

UINTERFACE(MinimalAPI, Blueprintable)
class UReactToTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class IReactToTriggerInterface
{
	GENERATED_BODY()

public:
	/** Add interface function declarations here */
	virtual FRPGConceptItemInfo GetMyStruct() { return FRPGConceptItemInfo::FRPGConceptItemInfo(); };
};

