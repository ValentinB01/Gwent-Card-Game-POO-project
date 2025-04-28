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
    
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    
    void triggerHeroAbility(Player& owner, Player& opponent, Board& board);
    void activateAbility(Player& owner, Player& opponent, Board& board) ;
    
    HeroAbility getAbility() const;
    int getAbilityValue() const;
};

#endif