#pragma once

#include "../Core/Player.h"
#include "../Core/Board.h"
#include "../Core/Deck.h"
#include <string>
#include <array>

class Game {
private:
    Board board;
    std::array<Player, 2> players;
    std::array<bool, 2> playerPassed;
    Deck deck;
    int currentRound = 1;
    bool gameOver = false;
    mutable bool newRoundFlag = false;
    std::vector<HeroAbility> currentAbilities;
    std::array<bool, 2> passedPlayers{false, false};
    int currentPlayerIndex = 0;
    bool abilityUsedThisRound = false;

public:
    Player& getOpponent();
    void markAbilityUsed(const std::string& heroName);
    Game(const std::string& player1Name, const std::string& player2Name);
    
    bool isNewRound() const;
    const std::vector<HeroAbility>& getHeroAbilities() const;
    void pass(int playerIndex);

    std::string getWinnerName() const;
    int getCurrentPlayerIndex() const;
    void update(float deltaTime);
    void loadDeck(const std::string& filename);
    void startGame();
    void nextRound();
    void playCard(int playerIndex, int cardIndex);
    void endTurn();
    bool isPlayerTurn(int playerIndex) const;

    void calculateRoundWinner();
    void printGameState() const;
    void resetPassStates();
    bool haveBothPlayersPassed() const;
    
    bool isGameOver() const;
    const Player& getCurrentPlayer() const;
    Player& getCurrentPlayer();
    const Player& getPlayer(int index) const;
    Player& getPlayer(int index);
    const Board& getBoard() const;
    Board& getBoard();
};
