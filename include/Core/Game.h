#ifndef GWENT_GAME_H
#define GWENT_GAME_H

#include "../Core/Player.h"
#include "../Core/Board.h"
#include "../Core/Deck.h"
#include <string>
#include <array>

class Game {
private:
    Board board;
    std::array<Player, 2> players;
    Deck deck;
    int currentRound;
    int currentPlayerIndex;
    bool gameOver;

public:
    Game(const std::string& player1Name, const std::string& player2Name);
    
    void loadDeck(const std::string& filename);
    void startGame();
    void nextRound();
    void playCard(int playerIndex, int cardIndex);
    void endTurn();
    void pass(int playerIndex);
    void calculateRoundWinner();
    void printGameState() const;
    
    // Getters
    bool isGameOver() const;
    const Player& getCurrentPlayer() const;
    const Player& getPlayer(int index) const;
    const Board& getBoard() const;
};

#endif