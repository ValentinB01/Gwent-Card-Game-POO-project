#ifndef GWENT_PLAYER_H
#define GWENT_PLAYER_H

#include "../Card/Card.h"
#include <vector>
#include <memory>
#include <string>

class Board;
class Deck;

class Player {
private:
    std::string name;
    int lifepoints;
    std::vector<std::unique_ptr<Card>> hand;
    int roundsWon;
    int playerId;
    Deck* deck;

public:
    Player(const std::string& name, int id, int startingLifepoints = 2);
    
    void setDeck(Deck* d);
    void drawCard();
    void drawCards(int count);
    void playCard(int index, Player& opponent, Board& board);
    void addCardToHand(std::unique_ptr<Card> card);
    void printHand() const;
    
    // Getters
    const std::string& getName() const;
    int getLifepoints() const;
    size_t getHandSize() const;
    int getRoundsWon() const;
    void winRound();
    int getPlayerId() const;
    int getRoundsLost() const;
    void loseLifepoint();
    void gainLifepoint();
    bool hasLost() const;
    void clearHand();
    void playCardToBoard(std::unique_ptr<Card> card, Board& board);
    std::vector<std::unique_ptr<Card>>& getGraveyard(Board& board);
};

#endif