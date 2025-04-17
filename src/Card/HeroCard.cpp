#include "../include/Card/HeroCard.h"
#include "../include/Core/Board.h"
#include "../include/Core/Player.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <algorithm>

HeroCard::HeroCard(const std::string& name, int power, CombatZone zone, 
                   Faction faction, HeroAbility ability, int abilityValue)
    : Card(name, power, CardType::HERO, zone, faction, 
           "Deploys a hero to the battlefield."),
      ability(ability), abilityValue(abilityValue) {}

void HeroCard::play(Player& owner, Player& opponent, Board& board) {
    std::cout << "⭐ Hero played: " << name << " | Power: " << power 
              << " | Zone: " << CardUtils::zoneToString(zone) 
              << " | Ability: " << CardUtils::heroAbilityToString(ability) << std::endl;
    
    auto heroCopy = std::make_unique<HeroCard>(*this);
    owner.playCardToBoard(std::move(heroCopy), board);
}

void HeroCard::applyEffect(Player& owner, Player& opponent, Board& board) {
    std::cout << name << " activates ability: " 
              << CardUtils::heroAbilityToString(ability) << std::endl;
    triggerHeroAbility(owner, opponent, board);
}

void HeroCard::triggerHeroAbility(Player& owner, Player& opponent, Board& board) {
    switch(ability) {
        case HeroAbility::COMMANDERS_HORN:
            board.doubleRowPower(owner.getPlayerId(), zone);
            std::cout << "Doubled power in " << CardUtils::zoneToString(zone) << " row!" << std::endl;
            break;
            
        case HeroAbility::SCORCH:
            board.destroyStrongestUnits(opponent.getPlayerId(), abilityValue);
            break;
            
        case HeroAbility::DECOY: {
            auto units = board.getPlayerUnits(owner.getPlayerId(), zone);
            if (!units.empty()) {
                std::cout << "Returned " << units[0]->getName() << " to hand" << std::endl;
            }
            break;
        }
            
        case HeroAbility::ALCHEMY: {
            auto units = board.getPlayerUnits(owner.getPlayerId());
            if (!units.empty()) {
                auto strongest = std::max_element(units.begin(), units.end(),
                    [](const Card* a, const Card* b) {
                        return a->getPower() < b->getPower();
                    });
                
                (*strongest)->setPower((*strongest)->getPower() + abilityValue);
                std::cout << "Boosted " << (*strongest)->getName() 
                         << " by " << abilityValue << std::endl;
            }
            break;
        }
            
        case HeroAbility::REVENGE:
            if (owner.getRoundsLost() > 0) {
                int boost = owner.getRoundsLost() * abilityValue;
                board.boostRow(owner.getPlayerId(), zone, boost);
                std::cout << "Revenge boost of " << boost << " applied!" << std::endl;
            }
            break;
    }
}