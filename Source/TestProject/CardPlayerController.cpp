#include "CardPlayerController.h"
#include "Engine/Engine.h"
#include "Engine/DataTable.h"

ACardPlayerController::ACardPlayerController()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // 設置默認值
    Health = 30;
    MaxHealth = 30;
    CurrentMana = 0;
    MaxMana = 0;
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
    Health = StartingHealth;
    MaxHealth = StartingHealth;
    CurrentMana = 0;
    MaxMana = 0;
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
    // 這只是一個示例，你可以根據需要修改
    for (int32 i = 0; i < AllCardData.Num() && DeckCards.Num() < 30; i++)
    {
        if (AllCardData[i])
        {
            // 每張卡加入兩張（如果是普通卡牌）
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
    OnHandUpdated();
    
    UE_LOG(LogTemp, Warning, TEXT("Drew initial hand of %d cards"), Hand.Num());
}

void ACardPlayerController::StartTurn()
{
    bIsMyTurn = true;
    TurnCount++;
    
    // 增加法力值
    GainMana();
    
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

void ACardPlayerController::GainMana()
{
    MaxMana = FMath::Min(MaxMana + 1, 10); // 最大法力值是10
    CurrentMana = MaxMana;
    OnManaUpdated();
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
        // 手牌滿了，燒掉抽到的卡
        UCard* BurnedCard = PlayerDeck->DrawCard();
        UE_LOG(LogTemp, Warning, TEXT("Card burned: hand is full"));
        return nullptr;
    }
    
    UCard* DrawnCard = PlayerDeck->DrawCard();
    if (DrawnCard)
    {
        Hand.Add(DrawnCard);
        OnCardDrawn(DrawnCard);
        OnHandUpdated();
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
    
    return Card->CanPlayCard(CurrentMana) && Hand.Contains(Card);
}

bool ACardPlayerController::PlayCard(UCard* Card)
{
    if (!CanPlayCard(Card))
    {
        return false;
    }
    
    // 扣除法力值
    CurrentMana -= Card->CardData.ManaCost;
    
    // 從手牌移除，添加到場上
    Hand.RemoveSingle(Card);
    PlayedCards.Add(Card);
    
    // 播放卡牌
    Card->PlayCard();
    
    // 更新UI
    OnCardPlayed(Card);
    OnHandUpdated();
    OnManaUpdated();
    
    return true;
}

void ACardPlayerController::TakeCardDamage(int32 Damage)
{
    Health = FMath::Max(0, Health - Damage);
    OnHealthUpdated();
    
    UE_LOG(LogTemp, Warning, TEXT("Took %d damage, health now: %d"), Damage, Health);
    
    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player defeated!"));
    }
}

void ACardPlayerController::Heal(int32 HealAmount)
{
    Health = FMath::Min(MaxHealth, Health + HealAmount);
    OnHealthUpdated();
    UE_LOG(LogTemp, Warning, TEXT("Healed %d, health now: %d"), HealAmount, Health);
}

bool ACardPlayerController::IsAlive() const
{
    return Health > 0;
}

void ACardPlayerController::DiscardCard(UCard* Card)
{
    if (Card && Hand.Contains(Card))
    {
        Hand.RemoveSingle(Card);
        OnHandUpdated();
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
        if (Card && Card->CanPlayCard(CurrentMana))
        {
            PlayableCards.Add(Card);
        }
    }
    
    return PlayableCards;
}
