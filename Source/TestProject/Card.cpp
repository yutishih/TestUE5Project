#include "Card.h"

UCard::UCard()
{
    CurrentPower = 0;
    bIsPlayable = false;
    bIsInPlay = false;
}

void UCard::InitializeCard(const FCardData& Data)
{
    CardData = Data;
    CurrentPower = Data.Power;
    bIsPlayable = true;
    bIsInPlay = false;
}

bool UCard::CanPlayCard(int32 CurrentMana) const
{
return bIsPlayable;
}

void UCard::PlayCard()
{
    if (bIsPlayable)
    {
        bIsInPlay = true;
        // 在這裡添加播放卡牌的邏輯
        UE_LOG(LogTemp, Warning, TEXT("Card played: %s"), *CardData.CardName);
    }
}

void UCard::TakeDamage(int32 Damage)
{
    if (bIsInPlay && CardData.CardType == ECardType::Creature)
    {
        CurrentPower = FMath::Max(0, CurrentPower - Damage);
        if (CurrentPower <= 0)
        {
            // 卡牌死亡邏輯
            UE_LOG(LogTemp, Warning, TEXT("Card destroyed: %s"), *CardData.CardName);
        }
    }
}

bool UCard::IsAlive() const
{
return CurrentPower > 0;
}

FString UCard::GetCardInfo() const
{
return FString::Printf(TEXT("Name: %s, Power: %d"), 
    *CardData.CardName, 
    CurrentPower
);
}
