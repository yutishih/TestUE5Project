#include "CardPlayerController.h"
#include "Engine/Engine.h"
#include "Engine/DataTable.h"

#include "CardPlayerController.h"
#include "Engine/Engine.h"
#include "Engine/DataTable.h"

// 新增 C++ 版手牌 UI 更新函式
void ACardPlayerController::OnHandUpdatedNative()
{
    // 這裡可以加上 C++ UI 更新邏輯
    // 最後觸發 Blueprint 事件
    OnHandUpdated();
    UE_LOG(LogTemp, Warning, TEXT("[Native] Hand updated, current hand size: %d"), Hand.Num());
}

ACardPlayerController::ACardPlayerController()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // 設置默認值（玩家無生命值）
    // 移除法力值相關
    MaxHandSize = 10;
    bIsMyTurn = false;
    TurnCount = 0;
    PlayerDeck = nullptr;
}

void ACardPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ACardPlayerController::InitializeForGame(int32 StartingHealth, UDataTable* CardDataTable)
{
    // 玩家無生命值
    // 移除法力值相關
    TurnCount = 0;
    bIsMyTurn = false;
    
    // 清空手牌和場上的卡牌
    Hand.Empty();
    PlayedCards.Empty();
    
    // 創建預設牌組
    CreateDefaultDeck(CardDataTable);
    
    UE_LOG(LogTemp, Warning, TEXT("Player initialized for game"));
}

void ACardPlayerController::CreateDefaultDeck(UDataTable* CardDataTable)
{
    if (!CardDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("CardDataTable is null, cannot create deck"));
        return;
    }
    
    PlayerDeck = NewObject<UDeck>();
    PlayerDeck->DeckName = TEXT("Default Deck");
    
    // 從數據表創建一個簡單的預設牌組
    TArray<FCardData*> AllCardData;
    CardDataTable->GetAllRows<FCardData>(TEXT("GetAllCardData"), AllCardData);
    
    TArray<FCardData> DeckCards;
    
    // 在這裡添加一些基礎卡牌到牌組中
    for (int32 i = 0; i < AllCardData.Num() && DeckCards.Num() < 30; i++)
    {
        if (AllCardData[i])
        {
            DeckCards.Add(*AllCardData[i]);
            if (AllCardData[i]->Rarity == ECardRarity::Common && DeckCards.Num() < 30)
            {
                DeckCards.Add(*AllCardData[i]);
            }
        }
    }
    
    if (DeckCards.Num() > 0)
    {
        PlayerDeck->InitializeDeck(DeckCards);
        UE_LOG(LogTemp, Warning, TEXT("Deck created with %d cards"), PlayerDeck->GetRemainingCards());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No cards found in data table, creating empty deck"));
    }
}

void ACardPlayerController::DrawInitialHand(int32 HandSize)
{
    if (!PlayerDeck)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerDeck is null, cannot draw initial hand"));
        return;
    }
    
    Hand = PlayerDeck->DrawHand(HandSize);
    OnHandUpdatedNative();
    
    UE_LOG(LogTemp, Warning, TEXT("Drew initial hand of %d cards"), Hand.Num());
}

void ACardPlayerController::StartTurn()
{
    bIsMyTurn = true;
    TurnCount++;
    
    // 抽一張卡（除了第一回合）
    if (TurnCount > 1)
    {
        DrawCard();
    }
    
    OnTurnStarted();
    UE_LOG(LogTemp, Warning, TEXT("Turn %d started"), TurnCount);
}

void ACardPlayerController::EndTurn()
{
    bIsMyTurn = false;
    OnTurnEnded();
    UE_LOG(LogTemp, Warning, TEXT("Turn ended"));
}

UCard* ACardPlayerController::DrawCard()
{
    if (!PlayerDeck || PlayerDeck->IsEmpty())
    {
        // 疲勞傷害邏輯
        TakeCardDamage(TurnCount - 30); // 如果牌組空了，造成疲勞傷害
        return nullptr;
    }

    if (Hand.Num() >= MaxHandSize)
    {
        UCard* BurnedCard = PlayerDeck->DrawCard();
        UE_LOG(LogTemp, Warning, TEXT("Card burned: hand is full"));
        return nullptr;
    }

    UCard* DrawnCard = PlayerDeck->DrawCard();
    if (DrawnCard)
    {
        Hand.Add(DrawnCard);
        OnCardDrawn(DrawnCard);
        OnHandUpdatedNative();
        UE_LOG(LogTemp, Warning, TEXT("Drew card: %s"), *DrawnCard->CardData.CardName);
    }

    return DrawnCard;
}

bool ACardPlayerController::CanPlayCard(UCard* Card)
{
    if (!Card || !bIsMyTurn)
    {
        return false;
    }

    return Card->bIsPlayable && Hand.Contains(Card);
}

bool ACardPlayerController::PlayCard(UCard* Card)
{
    if (!CanPlayCard(Card))
    {
        return false;
    }

    Hand.RemoveSingle(Card);
    PlayedCards.Add(Card);

    Card->PlayCard();

    OnCardPlayed(Card);
    OnHandUpdatedNative();

    return true;
}

void ACardPlayerController::TakeCardDamage(int32 Damage)
{
    UE_LOG(LogTemp, Warning, TEXT("Fatigue damage: %d (no player health)"), Damage);
}

void ACardPlayerController::Heal(int32 HealAmount)
{
    UE_LOG(LogTemp, Warning, TEXT("Heal called, but player has no health."));
}

bool ACardPlayerController::IsAlive() const
{
    return true;
}

void ACardPlayerController::DiscardCard(UCard* Card)
{
    if (Card && Hand.Contains(Card))
    {
        Hand.RemoveSingle(Card);
        OnHandUpdatedNative();
        UE_LOG(LogTemp, Warning, TEXT("Discarded card: %s"), *Card->CardData.CardName);
    }
}

int32 ACardPlayerController::GetHandSize() const
{
    return Hand.Num();
}

int32 ACardPlayerController::GetPlayedCardsCount() const
{
    return PlayedCards.Num();
}

TArray<UCard*> ACardPlayerController::GetPlayableCards() const
{
    TArray<UCard*> PlayableCards;
    for (UCard* Card : Hand)
    {
        if (Card && Card->bIsPlayable)
        {
            PlayableCards.Add(Card);
        }
    }
    return PlayableCards;
}
