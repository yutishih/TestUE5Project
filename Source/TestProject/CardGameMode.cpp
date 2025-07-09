#include "CardGameMode.h"
#include "CardPlayerController.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ACardGameMode::ACardGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // 設置默認值
    StartingHandSize = 4;
    MaxMana = 10;
    StartingHealth = 30;
    TurnTimeLimit = 75.0f; // 75秒回合時間
    CurrentPhase = EGamePhase::WaitingForPlayers;
    CurrentTurn = 1;
    CurrentPlayer = nullptr;
    CardDataTable = nullptr;
    
    // 設置默認的 PlayerController 類
    PlayerControllerClass = ACardPlayerController::StaticClass();
}

void ACardGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 延遲初始化遊戲，等待玩家加入
    FTimerHandle InitTimer;
    GetWorldTimerManager().SetTimer(InitTimer, this, &ACardGameMode::InitializeGame, 2.0f, false);
}

void ACardGameMode::InitializeGame()
{
    // 收集所有已連接的玩家
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        ACardPlayerController* CardPC = Cast<ACardPlayerController>(Iterator->Get());
        if (CardPC)
        {
            Players.Add(CardPC);
        }
    }
    
    if (Players.Num() >= 2)
    {
        StartGame();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Waiting for more players. Current players: %d"), Players.Num());
    }
}

void ACardGameMode::StartGame()
{
    if (Players.Num() < 2)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot start game with less than 2 players"));
        return;
    }
    
    CurrentPhase = EGamePhase::Mulligan;
    CurrentPlayer = Players[0];
    
    // 初始化每個玩家
    for (ACardPlayerController* Player : Players)
    {
        if (Player)
        {
            Player->InitializeForGame(StartingHealth, CardDataTable);
            Player->DrawInitialHand(StartingHandSize);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Card game started with %d players"), Players.Num());
    OnGameStarted();
    
    // 進入第一個玩家的回合
    NextPhase();
}

void ACardGameMode::NextPhase()
{
    switch (CurrentPhase)
    {
        case EGamePhase::Mulligan:
            CurrentPhase = EGamePhase::Player1Turn;
            CurrentPlayer = Players[0];
            break;
            
        case EGamePhase::Player1Turn:
            CurrentPhase = EGamePhase::Player2Turn;
            CurrentPlayer = Players[1];
            break;
            
        case EGamePhase::Player2Turn:
            CurrentPhase = EGamePhase::Player1Turn;
            CurrentPlayer = Players[0];
            CurrentTurn++;
            break;
            
        case EGamePhase::GameEnd:
            return;
            
        default:
            break;
    }
    
    if (CurrentPlayer)
    {
        CurrentPlayer->StartTurn();
        OnTurnChanged(CurrentPlayer);
        
        // 設置回合計時器
        GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ACardGameMode::HandleTurnTimeout, TurnTimeLimit, false);
    }
}

void ACardGameMode::EndTurn()
{
    if (CurrentPlayer)
    {
        CurrentPlayer->EndTurn();
    }
    
    // 清除回合計時器
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);
    
    SwitchToNextPlayer();
}

void ACardGameMode::SwitchToNextPlayer()
{
    NextPhase();
}

void ACardGameMode::EndGame(ACardPlayerController* Winner)
{
    CurrentPhase = EGamePhase::GameEnd;
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);
    
    UE_LOG(LogTemp, Warning, TEXT("Game ended. Winner: %s"), Winner ? TEXT("Player") : TEXT("Draw"));
    OnGameEnded(Winner);
}

bool ACardGameMode::CanPlayCard(UCard* Card, ACardPlayerController* Player)
{
    if (!Card || !Player || Player != CurrentPlayer)
    {
        return false;
    }
    
    if (CurrentPhase != EGamePhase::Player1Turn && CurrentPhase != EGamePhase::Player2Turn)
    {
        return false;
    }
    
    return Player->CanPlayCard(Card);
}

void ACardGameMode::PlayCard(UCard* Card, ACardPlayerController* Player)
{
    if (!CanPlayCard(Card, Player))
    {
        return;
    }
    
    Player->PlayCard(Card);
    OnCardPlayed(Card, Player);
    
    UE_LOG(LogTemp, Warning, TEXT("Card played: %s by Player"), *Card->CardData.CardName);
}

void ACardGameMode::HandleTurnTimeout()
{
    UE_LOG(LogTemp, Warning, TEXT("Turn timeout for current player"));
    EndTurn();
}
