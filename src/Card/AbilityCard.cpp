#include "../include/Card/AbilityCard.h"
#include "../include/Core/Board.h"
#include "../include/Core/Player.h"
#include "../include/Card/UnitCard.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <algorithm>

AbilityCard::AbilityCard(const std::string& name, CombatZone zone, Faction faction,
                       AbilityEffect effect, int effectValue, bool targetsEnemy)
    : Card(name, 0, CardType::ABILITY, zone, faction, "Special ability card"),
      effect(effect), effectValue(effectValue), _targetsEnemy(targetsEnemy) {}

void AbilityCard::play(Player& owner, Player& opponent, Board& board) {
    std::cout << "✨ Playing ability: " << name << " (" 
              << CardUtils::abilityEffectToString(effect) << ")\n";
    applyEffect(owner, opponent, board);
}

void AbilityCard::applyEffect(Player& owner, Player& opponent, Board& board) {
    Player& target = _targetsEnemy ? opponent : owner;
    
    switch(effect) {
        case AbilityEffect::DAMAGE_ROW:
            handleDamageRow(target, board);
            break;
            
        case AbilityEffect::CLEAR_SKIES:
            handleClearSkies(owner, board);
            break;
            
        case AbilityEffect::FOGLET_SPAWN:
            handleFogletSpawn(owner,zone, board);
            break;
            
        case AbilityEffect::COMMANDO_TRAINING:
            handleCommandoTraining(owner, board);
            break;
            
        case AbilityEffect::VENOM_EXTRACT:
            handleVenomExtract(target, board);
            break;
            
        default:
            std::cerr << "Unknown ability effect!\n";
    }
}

void AbilityCard::handleDamageRow(Player& target, Board& board) {
    auto& units = board.getPlayerZone(target.getPlayerId(), zone);
    int damagedUnits = 0;
    int destroyedUnits = 0;

    for (auto& unit : units) {
        int originalPower = unit->getPower();
        unit->takeDamage(effectValue);
        damagedUnits++;

        std::cout << "💥 Damaged " << unit->getName() 
                 << " (Power: " << originalPower << " → " 
                 << unit->getPower() << ")\n";

        if (unit->getPower() <= 0) {
            destroyedUnits++;
        }
    }

    if (destroyedUnits > 0) {
        board.cleanupDestroyedUnits(target.getPlayerId(), zone);
    }

    std::cout << "Total damaged: " << damagedUnits << " units in " 
             << CardUtils::zoneToString(zone) << " zone\n";
    if (destroyedUnits > 0) {
        std::cout << "💀 Destroyed " << destroyedUnits << " units\n";
    }
}

void AbilityCard::handleClearSkies(Player& owner, Board& board) {
    board.clearWeather();
    
    if (board.hasUnitsInZone(owner.getPlayerId(), zone)) {
        board.boostRow(owner.getPlayerId(), zone, effectValue);
        std::cout << "☀️ Cleared weather and boosted " 
                 << CardUtils::zoneToString(zone) << " by " 
                 << effectValue << ".\n";
    } else {
        std::cout << "☀️ Cleared weather (no units in " 
                 << CardUtils::zoneToString(zone) << ").\n";
    }
}


void AbilityCard::handleFogletSpawn(Player& owner,CombatZone zone, Board& board) {
    if (board.hasWeather(WeatherType::IMPENETRABLE_FOG)) {
        owner.playCardToBoard(
            std::make_unique<UnitCard>("Foglet", 2, zone, Faction::MONSTERS),
            board
        );
        std::cout << "👻 Summoned a Foglet!\n";
    } else {
        std::cout << "No fog weather - Foglet not summoned\n";
    }
}

void AbilityCard::handleCommandoTraining(Player& owner, Board& board) {
    auto& zoneUnits = board.getPlayerZone(owner.getPlayerId(), zone);
    int boostedUnits = 0;
    
    for (auto& unit : zoneUnits) {
        unit->setPower(unit->getPower() + effectValue);
        boostedUnits++;
        std::cout << "Boosted " << unit->getName() 
                 << " by " << effectValue << " (New power: " 
                 << unit->getPower() << ").\n";
    }
    
    if (boostedUnits == 0) {
        std::cout << "No units in " << CardUtils::zoneToString(zone) 
                 << " zone to boost\n";
    } else {
        std::cout << "Total boosted: " << boostedUnits << " units in " 
                 << CardUtils::zoneToString(zone) << " zone.\n";
    }
}

void AbilityCard::handleVenomExtract(Player& target, Board& board) {
    auto units = board.getPlayerUnits(target.getPlayerId());
    
    if (!units.empty()) {
        auto strongest = std::max_element(units.begin(), units.end(),
            [](const Card* a, const Card* b) {
                return a->getPower() < b->getPower();
            });
        
        (*strongest)->takeDamage(effectValue);
        std::cout << "☠️ Poisoned " << (*strongest)->getName() 
                 << " for " << effectValue << " damage.\n";
        
        if ((*strongest)->getPower() <= 0) {
            std::cout << "💀 " << (*strongest)->getName() << " was destroyed!\n";
        }
    } else {
        std::cout << "No units to poison\n";
    }
}