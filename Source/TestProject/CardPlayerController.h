#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Card.h"
#include "Deck.h"
#include "CardPlayerController.generated.h"

UCLASS()
class TESTPROJECT_API ACardPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACardPlayerController();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    int32 Health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    int32 MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    int32 CurrentMana;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    int32 MaxMana;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
    TArray<UCard*> Hand;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
    TArray<UCard*> PlayedCards;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
    UDeck* PlayerDeck;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
    int32 MaxHandSize;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
    bool bIsMyTurn;

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void InitializeForGame(int32 StartingHealth, class UDataTable* CardDataTable);

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void DrawInitialHand(int32 HandSize);

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void StartTurn();

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void EndTurn();

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    UCard* DrawCard();

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    bool CanPlayCard(UCard* Card);

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    bool PlayCard(UCard* Card);

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void TakeCardDamage(int32 Damage);

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void Heal(int32 HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void DiscardCard(UCard* Card);

    UFUNCTION(BlueprintCallable, Category = "Player Query")
    int32 GetHandSize() const;

    UFUNCTION(BlueprintCallable, Category = "Player Query")
    int32 GetPlayedCardsCount() const;

    UFUNCTION(BlueprintCallable, Category = "Player Query")
    TArray<UCard*> GetPlayableCards() const;

    // UI 相關的 Blueprint 事件
    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnHandUpdated();

    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnManaUpdated();

    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnHealthUpdated();

    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnTurnStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnTurnEnded();

    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnCardDrawn(UCard* DrawnCard);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI Events")
    void OnCardPlayed(UCard* PlayedCard);

protected:
    UFUNCTION()
    void CreateDefaultDeck(class UDataTable* CardDataTable);

    UFUNCTION()
    void GainMana();

private:
    int32 TurnCount;
};
