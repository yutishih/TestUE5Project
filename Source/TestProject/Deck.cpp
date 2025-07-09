#include "Deck.h"
#include "Engine/Engine.h"

UDeck::UDeck()
{
    MaxDeckSize = 30; // 類似爐石傳說的牌組大小
    DeckName = TEXT("Default Deck");
}

void UDeck::InitializeDeck(const TArray<FCardData>& CardDataArray)
{
    Cards.Empty();
    
    for (const FCardData& CardData : CardDataArray)
    {
        UCard* NewCard = NewObject<UCard>();
        NewCard->InitializeCard(CardData);
        Cards.Add(NewCard);
    }
    
    Shuffle();
}

bool UDeck::AddCard(UCard* NewCard)
{
    if (!NewCard || Cards.Num() >= MaxDeckSize)
    {
        return false;
    }
    
    Cards.Add(NewCard);
    return true;
}

bool UDeck::RemoveCard(UCard* CardToRemove)
{
    if (!CardToRemove)
    {
        return false;
    }
    
    return Cards.RemoveSingle(CardToRemove) > 0;
}

UCard* UDeck::DrawCard()
{
    if (Cards.Num() == 0)
    {
        return nullptr;
    }
    
    UCard* DrawnCard = Cards[0];
    Cards.RemoveAt(0);
    return DrawnCard;
}

void UDeck::Shuffle()
{
    if (Cards.Num() <= 1)
    {
        return;
    }
    
    // Fisher-Yates 洗牌算法
    for (int32 i = Cards.Num() - 1; i > 0; i--)
    {
        int32 j = FMath::RandRange(0, i);
        Cards.Swap(i, j);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Deck shuffled: %s"), *DeckName);
}

int32 UDeck::GetRemainingCards() const
{
    return Cards.Num();
}

bool UDeck::IsEmpty() const
{
    return Cards.Num() == 0;
}

TArray<UCard*> UDeck::DrawHand(int32 HandSize)
{
    TArray<UCard*> Hand;
    
    for (int32 i = 0; i < HandSize && !IsEmpty(); i++)
    {
        UCard* DrawnCard = DrawCard();
        if (DrawnCard)
        {
            Hand.Add(DrawnCard);
        }
    }
    
    return Hand;
}

bool UDeck::IsValidDeck() const
{
    // 檢查牌組是否符合遊戲規則
    if (Cards.Num() < 20 || Cards.Num() > MaxDeckSize)
    {
        return false;
    }
    
    // 可以在這裡添加更多的牌組驗證規則
    // 例如：每張卡最多只能有幾張等等
    
    return true;
}
