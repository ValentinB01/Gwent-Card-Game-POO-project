#ifndef GWENT_HEROCARD_H
#define GWENT_HEROCARD_H

#include "../Card/Card.h"
#include "../Utils/enums.h"

class HeroCard : public Card {
private:
    HeroAbility ability;
    int abilityValue;

public:
    HeroCard(const std::string& name, int power, CombatZone zone, 
             Faction faction, HeroAbility ability, int abilityValue = 0);
    
    // Overridden virtual functions
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    
    // Ability activation
    void triggerHeroAbility(Player& owner, Player& opponent, Board& board);
    
    // Getters
    HeroAbility getAbility() const;
    int getAbilityValue() const;
};

#endif