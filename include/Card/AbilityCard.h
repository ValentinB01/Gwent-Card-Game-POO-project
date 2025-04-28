#pragma once

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
    
private:
    void handleDamageZone(Player& target, Board& board);
    void handleDamageRow(Player& target, Board& board);
    void handleClearSkies(Player& owner, Board& board);
    void handleFogletSpawn(Player& owner, CombatZone zone, Board& board);
    void handleCommandoTraining(Player& owner, Board& board);
    void handleVenomExtract(Player& target, Board& board);

    AbilityEffect effect;
    int effectValue;
    bool _targetsEnemy;
};