#ifndef GWENT_PLAYER_H
#define GWENT_PLAYER_H

#include "../Card/Card.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <string>

class Board;
class Deck;

class Player {
private:
    std::string name;
    int lifepoints;
    std::vector<std::unique_ptr<Card>> hand;
    std::unordered_set<std::string> usedHeroAbilitiesThisRound;    
    std::vector<std::unique_ptr<Card>> graveyard;
    int roundsWon;
    int playerId;
    Deck* deck;

public:
    Player(const std::string& name, int id, int startingLifepoints = 2);
    
    const std::vector<std::unique_ptr<Card>>& getGraveyard() const;  
    size_t getGraveyardSize() const;
    void discardCard(size_t handIndex);
    void activateHeroAbility(Board& board, Player& opponent);
    void resetHeroAbilitiesForNewRound();
    bool canUseHeroAbility(const std::string& heroName) const;
    void markHeroAbilityUsed(const std::string& heroName);

    void setDeck(Deck* d);
    void drawCard();
    void drawCards(int count);
    void playCard(int index, Player& opponent, Board& board);
    void addCardToHand(std::unique_ptr<Card> card);
    void printHand() const;
    
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