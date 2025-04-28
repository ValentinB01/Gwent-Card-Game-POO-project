#include "../include/Card/HeroCard.h"
#include "../include/Core/Board.h"
#include "../include/Core/Player.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <algorithm>

HeroCard::HeroCard(const std::string& name, int power, CombatZone zone, 
                   Faction faction, HeroAbility ability, int abilityValue)
    : Card(name, power, CardType::HERO, zone, faction, 
           "Deploys a hero to the battlefield with the ability: "+ CardUtils::heroAbilityToString(ability)),
      ability(ability), abilityValue(abilityValue) {}

void HeroCard::play(Player& owner, Player& opponent, Board& board) {
    std::cout << "⭐ Hero played: " << name << " | Power: " << power 
              << " | Zone: " << CardUtils::zoneToString(zone) 
              << " | Ability: " << CardUtils::heroAbilityToString(ability) << std::endl;
    
    auto heroCopy = std::make_unique<HeroCard>(*this);
    owner.playCardToBoard(std::move(heroCopy), board);
}

void HeroCard::activateAbility(Player& owner, Player& opponent, Board& board) {
    if (owner.canUseHeroAbility(getName())) {
        applyEffect(owner, opponent, board);
        owner.markHeroAbilityUsed(getName());
        std::cout << getName() << "'s ability activated successfully!\n";
    } else {
        throw std::runtime_error(getName() + "'s ability already used this round");
    }
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
            std::cout << "Doubled power in " << CardUtils::zoneToString(zone) << " battle zone!" << std::endl;
            break;
            
            case HeroAbility::SCORCH: {
                auto result = board.destroyStrongestEnemyUnit(owner.getPlayerId(), this);
                
                if (!result.destroyedName.empty()) {
                    std::cout << "🔥 " << name << " scorched ";
                    if (result.wasHero) {
                        std::cout << "enemy hero. ";
                    }
                    std::cout << result.destroyedName << " (Power: " << result.power 
                              << ") from " << CardUtils::zoneToString(result.zone) << " battle zone!\n";
                } else {
                    std::cout << "No valid enemy units to scorch!\n";
                }
                break;
            }
            
            case HeroAbility::DECOY: {
                try {
                    auto& zoneCards = board.getPlayerZone(owner.getPlayerId(), zone);
                    if (zoneCards.empty()) {
                        std::cout << "No cards in zone to decoy!\n";
                        break;
                    }
            
                    if (!zoneCards[0]) {
                        std::cerr << "Error: Null card in zone!\n";
                        break;
                    }
            
                    std::string cardName = zoneCards[0]->getName();
                    CardType cardType = zoneCards[0]->getType();
            
                    owner.addCardToHand(std::move(zoneCards[0]));
                    zoneCards.erase(zoneCards.begin());
                    
                    std::cout << "Decoy returned " << cardName 
                              <<"to hand\n";
                } 
                catch (const std::exception& e) {
                    std::cerr << "Decoy failed: " << e.what() << "\n";
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
HeroAbility HeroCard::getAbility() const {
    return ability;
}