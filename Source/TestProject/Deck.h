#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Card.h"
#include "Deck.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TESTPROJECT_API UDeck : public UObject
{
    GENERATED_BODY()

public:
    UDeck();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
    TArray<UCard*> Cards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
    int32 MaxDeckSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
    FString DeckName;

    UFUNCTION(BlueprintCallable, Category = "Deck")
    void InitializeDeck(const TArray<FCardData>& CardDataArray);

    UFUNCTION(BlueprintCallable, Category = "Deck")
    bool AddCard(UCard* NewCard);

    UFUNCTION(BlueprintCallable, Category = "Deck")
    bool RemoveCard(UCard* CardToRemove);

    UFUNCTION(BlueprintCallable, Category = "Deck")
    UCard* DrawCard();

    UFUNCTION(BlueprintCallable, Category = "Deck")
    void Shuffle();

    UFUNCTION(BlueprintCallable, Category = "Deck")
    int32 GetRemainingCards() const;

    UFUNCTION(BlueprintCallable, Category = "Deck")
    bool IsEmpty() const;

    UFUNCTION(BlueprintCallable, Category = "Deck")
    TArray<UCard*> DrawHand(int32 HandSize);

    UFUNCTION(BlueprintCallable, Category = "Deck")
    bool IsValidDeck() const;
};
