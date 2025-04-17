#ifndef GWENT_ABILITYCARD_H
#define GWENT_ABILITYCARD_H

#include "../Card/Card.h"
#include "../Utils/enums.h"

class AbilityCard : public Card {
private:
    AbilityEffect effect;
    int effectValue;
    bool targetsEnemy;

public:
    AbilityCard(const std::string& name, CombatZone zone, Faction faction, 
               AbilityEffect effect, int effectValue = 0, bool targetsEnemy = true);
    
    // Overridden virtual functions
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    
    // Static helper
    static std::string abilityEffectToString(AbilityEffect effect);
    
    // Getters
    AbilityEffect getEffect() const;
    int getEffectValue() const;
    bool getTargetsEnemy() const;
};

#endif