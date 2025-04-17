#include "../include/Card/AbilityCard.h"
#include "../include/Core/Board.h"
#include "../include/Core/Player.h"
#include "../include/Utils/CardUtils.h"
#include "../include/Card/UnitCard.h"
#include <iostream>
#include <algorithm>

AbilityCard::AbilityCard(const std::string& name, CombatZone zone, Faction faction, 
                        AbilityEffect effect, int effectValue, bool targetsEnemy)
    : Card(name, 0, CardType::ABILITY, zone, faction, "Special ability card"),
      effect(effect), effectValue(effectValue), targetsEnemy(targetsEnemy) {}

void AbilityCard::play(Player& owner, Player& opponent, Board& board) {
    std::cout << "✨ Ability played: " << name << " | Effect: " 
              << CardUtils::abilityEffectToString(effect) << " | Zone: " 
              << CardUtils::zoneToString(zone) << std::endl;
    applyEffect(owner, opponent, board);
}

void AbilityCard::applyEffect(Player& owner, Player& opponent, Board& board) {
    Player& target = targetsEnemy ? opponent : owner;
    
    switch(effect) {
        case AbilityEffect::DAMAGE_ROW:
            board.damageRow(target.getPlayerId(), zone, effectValue);
            std::cout << "Damaged " << CardUtils::zoneToString(zone) 
                     << " row by " << effectValue << std::endl;
            break;
            
        case AbilityEffect::CLEAR_SKIES:
            board.clearWeather();
            board.boostRow(owner.getPlayerId(), zone, effectValue);
            std::cout << "Weather cleared and row boosted by " << effectValue << std::endl;
            break;
            
        case AbilityEffect::FOGLET_SPAWN:
            if (board.hasWeather(WeatherType::IMPENETRABLE_FOG)) {
                owner.playCardToBoard(
                    std::make_unique<UnitCard>("Foglet", 2, zone, Faction::MONSTERS),
                    board
                );
                std::cout << "Summoned a Foglet!" << std::endl;
            }
            break;
            
        case AbilityEffect::COMMANDO_TRAINING: {
            auto units = board.getPlayerUnits(owner.getPlayerId());
            for (auto unit : units) {
                if (unit->getName().find("Commando") != std::string::npos) {
                    unit->setPower(unit->getPower() + effectValue);
                    std::cout << "Boosted " << unit->getName() 
                             << " by " << effectValue << std::endl;
                }
            }
            break;
        }
            
        case AbilityEffect::VENOM_EXTRACT: {
            auto units = board.getPlayerUnits(target.getPlayerId());
            if (!units.empty()) {
                auto strongest = std::max_element(units.begin(), units.end(),
                    [](const Card* a, const Card* b) {
                        return a->getPower() < b->getPower();
                    });
                
                (*strongest)->takeDamage(effectValue);
                std::cout << "Poisoned " << (*strongest)->getName() 
                         << " for " << effectValue << " damage" << std::endl;
            }
            break;
        }
    }
}