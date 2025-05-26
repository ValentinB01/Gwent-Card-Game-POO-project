#pragma once

#include "../Card/Card.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <string>

class Board;
class Deck;
class Game;
class GameWindow;
class Hero {
    public:
        std::vector<HeroAbility> abilities;
    };

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
    int selectedCardIndex = -1;
    GameWindow* gameWindow = nullptr;
    Hero hero;
    Player* opponent = nullptr;
    Game* game = nullptr;

public:
    Player(const std::string& name, int id, int startingLifepoints = 2);
    void setOpponent(Player* opp);
    void setGameReference(Game* gameRef);
    Player& getOpponent() const;
    
    void setGameWindow(GameWindow* window);
    GameWindow* getGameWindow() const;
    const Hero& getHero() const;
    const std::vector<HeroAbility>& getHeroAbilities() const;
    std::vector<HeroAbility>& getHeroAbilities();

    void drawCard();
    void drawCards(int count);
    void playCard(int index, Player& opponent, Board& board);
    void discardCard(size_t handIndex);
    void addCardToHand(std::unique_ptr<Card> card);


    void selectCard(int index);
    void deselectCard();
    int getSelectedCardIndex() const;
    const Card* getSelectedCard() const;

    bool canUseHeroAbility(const std::string& heroName) const;
    bool canUseHeroAbility() const;
    void markHeroAbilityUsed(const std::string& heroName);
    void resetHeroAbilitiesForNewRound();
    void activateHeroAbility(Board& board, Player& opponent);

    void playCardToBoard(std::unique_ptr<Card> card, Board& board);


    const std::vector<std::unique_ptr<Card>>& getHand() const;
    const std::vector<std::unique_ptr<Card>>& getGraveyard() const;  
    size_t getGraveyardSize() const;
    std::vector<std::unique_ptr<Card>>& getGraveyard(Board& board);

    

    void setDeck(Deck* d);    
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
};
