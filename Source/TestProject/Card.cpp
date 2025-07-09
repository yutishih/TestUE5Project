#include "Card.h"

UCard::UCard()
{
    CurrentHealth = 0;
    CurrentAttack = 0;
    bIsPlayable = false;
    bIsInPlay = false;
}

void UCard::InitializeCard(const FCardData& Data)
{
    CardData = Data;
    CurrentHealth = Data.Health;
    CurrentAttack = Data.Attack;
    bIsPlayable = true;
    bIsInPlay = false;
}

bool UCard::CanPlayCard(int32 CurrentMana) const
{
    return bIsPlayable && CurrentMana >= CardData.ManaCost;
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
        CurrentHealth = FMath::Max(0, CurrentHealth - Damage);
        if (CurrentHealth <= 0)
        {
            // 卡牌死亡邏輯
            UE_LOG(LogTemp, Warning, TEXT("Card destroyed: %s"), *CardData.CardName);
        }
    }
}

bool UCard::IsAlive() const
{
    return CurrentHealth > 0;
}

FString UCard::GetCardInfo() const
{
    return FString::Printf(TEXT("Name: %s, Cost: %d, Attack: %d, Health: %d"), 
        *CardData.CardName, 
        CardData.ManaCost, 
        CurrentAttack, 
        CurrentHealth
    );
}
