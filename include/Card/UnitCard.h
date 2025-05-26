#pragma once

#include <SFML/Graphics.hpp>
#include "../Card/Card.h"
#include "../Utils/enums.h"

class UnitCard : public Card {
private:
    bool isHero;
    DeployEffect deployEffect;
    int effectValue;
    bool isSpy;
    int basePower;

public:
    UnitCard(const std::string& name, int power, CombatZone zone, 
             Faction faction, bool isHero = false,
             DeployEffect effect = DeployEffect::NONE, 
             int effectValue = 0, bool isSpy = false);
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    void triggerDeployEffect(Player& owner, Player& opponent, Board& board);
    sf::FloatRect getGlobalBounds() const;

    int getBasePower() const;
    bool isHeroCard() const;
    DeployEffect getDeployEffect() const;
    int getEffectValue() const;
};
