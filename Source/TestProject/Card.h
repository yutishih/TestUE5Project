#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"

UENUM(BlueprintType)
enum class ECardType : uint8
{
    None        UMETA(DisplayName = "None"),
    Creature    UMETA(DisplayName = "Creature"),
    Spell       UMETA(DisplayName = "Spell"),
    Equipment   UMETA(DisplayName = "Equipment"),
    Trap        UMETA(DisplayName = "Trap")
};

UENUM(BlueprintType)
enum class ECardRarity : uint8
{
    Common      UMETA(DisplayName = "Common"),
    Uncommon    UMETA(DisplayName = "Uncommon"),
    Rare        UMETA(DisplayName = "Rare"),
    Epic        UMETA(DisplayName = "Epic"),
    Legendary   UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    int32 CardID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    FString CardName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    ECardType CardType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    ECardRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    int32 ManaCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    int32 Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    class UTexture2D* CardImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
    TArray<FString> Tags;

    FCardData()
    {
        CardID = 0;
        CardName = TEXT("Unknown Card");
        Description = TEXT("No description available");
        CardType = ECardType::None;
        Rarity = ECardRarity::Common;
        ManaCost = 0;
        Attack = 0;
        Health = 0;
        CardImage = nullptr;
    }
};

UCLASS(BlueprintType, Blueprintable)
class TESTPROJECT_API UCard : public UObject
{
    GENERATED_BODY()

public:
    UCard();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FCardData CardData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card State")
    int32 CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card State")
    int32 CurrentAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card State")
    bool bIsPlayable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card State")
    bool bIsInPlay;

    UFUNCTION(BlueprintCallable, Category = "Card")
    void InitializeCard(const FCardData& Data);

    UFUNCTION(BlueprintCallable, Category = "Card")
    bool CanPlayCard(int32 CurrentMana) const;

    UFUNCTION(BlueprintCallable, Category = "Card")
    void PlayCard();

    UFUNCTION(BlueprintCallable, Category = "Card")
    void TakeDamage(int32 Damage);

    UFUNCTION(BlueprintCallable, Category = "Card")
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "Card")
    FString GetCardInfo() const;
};
