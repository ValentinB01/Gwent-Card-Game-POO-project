#include "../include/Card/UnitCard.h"
#include "../include/Core/Player.h"
#include "../include/Core/Board.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <algorithm>

UnitCard::UnitCard(const std::string& name, int power, CombatZone zone, 
                   Faction faction, bool isHero, DeployEffect effect, 
                   int effectValue, bool isSpy)
    : Card(name, power, CardType::UNIT, zone, faction, 
           "Deploys a unit card with the deploy effect: " + CardUtils::deployEffectToString(effect)),
      isHero(isHero), deployEffect(effect), 
      effectValue(effectValue), isSpy(isSpy) {}

      void UnitCard::play(Player& owner, Player& opponent, Board& board) {
        auto unitCopy = std::make_unique<UnitCard>(*this);
        owner.playCardToBoard(std::move(unitCopy), board);
    }

void UnitCard::applyEffect(Player& owner, Player& opponent, Board& board) {
    triggerDeployEffect(owner, opponent, board);
}

void UnitCard::triggerDeployEffect(Player& owner, Player& opponent, Board& board) {
    switch(deployEffect) {
        case DeployEffect::DAMAGE_RANDOM_ENEMY: {
            auto units = board.getPlayerUnits(opponent.getPlayerId());
            if (!units.empty()) {
                int randomIndex = rand() % units.size();
                units[randomIndex]->takeDamage(effectValue);
                std::cout << "Dealt " << effectValue << " damage to " 
                     << units[randomIndex]->getName() <<"."<< std::endl;
            }
            break;
        }
            
        case DeployEffect::BOOST_ADJACENT: {
            auto units = board.getPlayerUnits(owner.getPlayerId(), zone);
            for (auto unit : units) {
                if (unit->getName() != name) {
                    unit->setPower(unit->getPower() + effectValue);
                    std::cout << "Boosted " << unit->getName() 
                         << " by " << effectValue <<"."<< std::endl;
                }
            }
            break;
        }
            
        case DeployEffect::DRAW_CARD:
            owner.drawCards(effectValue);
            std::cout << "Drew " << effectValue << " card(s)." << std::endl;
            break;
            
            case DeployEffect::DESTROY_WEAKEST: {
                std::string destroyedName = board.destroyWeakestUnit(opponent.getPlayerId());
                if (!destroyedName.empty()) {
                    std::cout << "Destroyed opponent's weakest unit: " << destroyedName <<".";
                } else {
                    std::cout << "No units to destroy!\n";
                }
                break;
            }
            
        case DeployEffect::CLEAR_WEATHER:
            board.clearWeather();
            std::cout << "Weather effects cleared!" << std::endl;
            break;
            
        case DeployEffect::SPY:
            break;
            
        case DeployEffect::MEDIC: {
            auto& graveyard = board.getPlayerGraveyard(owner.getPlayerId());
            if (!graveyard.empty()) {
                auto revived = move(graveyard.back());
                graveyard.pop_back();
                std::cout << "Revived " << revived->getName() << std::endl;
                owner.playCardToBoard(move(revived), board);
            }
            break;
        }
            
        case DeployEffect::MORALE_BOOST:
            board.boostRow(owner.getPlayerId(), zone, effectValue);
            std::cout << "Morale boost applied to the "<<CardUtils::zoneToString(zone) <<" battle zone!" << std::endl;
            break;
            
        case DeployEffect::NONE:
        default:
            break;
    }
}
    bool UnitCard::isHeroCard() const {
        return isHero;
    }
    DeployEffect UnitCard::getDeployEffect() const {
        return deployEffect;
    }
    int UnitCard::getEffectValue() const {
        return effectValue;
    }
    bool UnitCard::getIsSpy() const {
        return isSpy;
    }
    int UnitCard::getBasePower() const { 
        return basePower; 
    }
sf::FloatRect UnitCard::getGlobalBounds() const {
    return sf::FloatRect(position.x, position.y, size.x, size.y);
}
