#pragma once

#include <SFML/Graphics.hpp>
#include "../Card/Card.h"
#include "../Utils/enums.h"
class Player;
class Board;

class AbilityCard : public Card {
public:
    AbilityCard(const std::string& name, CombatZone zone, Faction faction, 
               AbilityEffect effect, int effectValue, bool targetsEnemy);
    
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    AbilityEffect getEffect() const { return effect; }
    int getEffectValue() const { return effectValue; }
    sf::FloatRect getGlobalBounds() const;


private:
    void handleDamageRow(Player& target, Board& board);
    void handleClearSkies(Player& owner, Board& board);
    void handleFogletSpawn(Player& owner, CombatZone zone, Board& board);
    void handleCommandoTraining(Player& owner, Board& board);
    void handleVenomExtract(Player& target, Board& board);

    AbilityEffect effect;
    int effectValue;
    bool _targetsEnemy;
};
