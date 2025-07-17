#pragma once
#include "TestProject/CardPlayerController.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Card.h"
#include "Deck.h"
#include "CardGameMode.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
    WaitingForPlayers   UMETA(DisplayName = "Waiting for Players"),
    Mulligan           UMETA(DisplayName = "Mulligan"),
    Player1Turn        UMETA(DisplayName = "Player 1 Turn"),
    Player2Turn        UMETA(DisplayName = "Player 2 Turn"),
    GameEnd            UMETA(DisplayName = "Game End")
};

UCLASS()
class TESTPROJECT_API ACardGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACardGameMode();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
    int32 StartingHandSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
    int32 StartingHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
    float TurnTimeLimit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    EGamePhase CurrentPhase;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 CurrentTurn;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    class ACardPlayerController* CurrentPlayer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    TArray<class ACardPlayerController*> Players;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    class UDataTable* CardDataTable;

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndTurn();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void NextPhase();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndGame(ACardPlayerController* Winner);

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    bool CanPlayCard(UCard* Card, ACardPlayerController* Player);

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void PlayCard(UCard* Card, ACardPlayerController* Player);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnGameStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnTurnChanged(ACardPlayerController* NewCurrentPlayer);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnCardPlayed(UCard* PlayedCard, ACardPlayerController* Player);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnGameEnded(ACardPlayerController* Winner);

protected:
    UFUNCTION()
    void InitializeGame();

    UFUNCTION()
    void SwitchToNextPlayer();

private:
    FTimerHandle TurnTimerHandle;
    
    UFUNCTION()
    void HandleTurnTimeout();
};
