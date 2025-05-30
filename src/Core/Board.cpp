#include "../include/Core/Board.h"
#include "../include/Card/UnitCard.h"
#include "../include/Card/WeatherCard.h"
#include "../include/Utils/CardUtils.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <limits>
#include <random>

Board* Board::instance = nullptr;

Board* Board::getInstance() {
    if(!instance)
        instance = new Board();
    return instance;
}

void Board::addCard(int playerIndex, std::unique_ptr<Card> card) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    if (card->getType() == CardType::WEATHER) {
        auto weather = dynamic_cast<WeatherCard*>(card.get());
        if (weather) {
            addWeather(std::move(card));
            return;
        }
    }
    playerBoards[playerIndex].zones[card->getZone()].push_back(std::move(card));
}

std::vector<std::unique_ptr<Card>>& Board::getPlayerZone(int playerIndex, CombatZone zone) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }
    
    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone) == 0) {
        zones[zone] = std::vector<std::unique_ptr<Card>>();
    }
    return zones[zone];
}

const std::vector<std::unique_ptr<Card>>& Board::getPlayerZone(int playerIndex, CombatZone zone) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }
    
    static const std::vector<std::unique_ptr<Card>> emptyZone;
    const auto& zones = playerBoards[playerIndex].zones;
    
    if (zones.count(zone)) {
        return zones.at(zone);
    }
    return emptyZone;
}

std::vector<std::unique_ptr<Card>>& Board::getPlayerGraveyard(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }
    return playerBoards[playerIndex].graveyard;
}

const std::vector<std::unique_ptr<Card>>& Board::getPlayerGraveyard(int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }
    return playerBoards[playerIndex].graveyard;
}


void Board::addWeather(std::unique_ptr<Card> weatherCard) {
    if (auto weather = dynamic_cast<WeatherCard*>(weatherCard.get())) {
        weatherEffects.erase(
            remove_if(weatherEffects.begin(), weatherEffects.end(),
                [&weather](const auto& w) {
                    if (auto wc = dynamic_cast<const WeatherCard*>(w.get())) {
                        if (wc->getWeatherType() == weather->getWeatherType()) {
                            for (auto zone : wc->getAffectedZones()) {
                                if (weather->affectsZone(zone)) {
                                    return true;
                                }
                            }
                            return false;
                        }
                    }
                    return false;
                }),
            weatherEffects.end());
                    
        for (auto zone : weather->getAffectedZones()) {
            if (zone == CombatZone::ANY) {
                applyWeather(weather->getWeatherType(), {CombatZone::CLOSE, CombatZone::RANGED, CombatZone::SIEGE}, weather->getEffectValue());
            } else {
                applyWeather(weather->getWeatherType(), {zone}, weather->getEffectValue());
            }
        }
    }
}

bool Board::shouldApplyWeather(CombatZone weatherZone, CombatZone targetZone) const {
    return weatherZone == CombatZone::ANY || weatherZone == targetZone;
}

const WeatherCard* Board::getActiveWeatherForZone(CombatZone zone) const {
    const WeatherCard* zoneSpecificWeather = nullptr;
    const WeatherCard* globalWeather = nullptr;

    for (const auto& weather : weatherEffects) {
        if (auto wc = dynamic_cast<const WeatherCard*>(weather.get())) {
            if (wc->getZone() == zone) {
                zoneSpecificWeather = wc;
            }
            else if (wc->getZone() == CombatZone::ANY) {
                globalWeather = wc;
            }
        }
    }

    return zoneSpecificWeather ? zoneSpecificWeather : globalWeather;
}

WeatherType Board::getWeatherType(CombatZone zone) const {
    if (const WeatherCard* weather = getActiveWeatherForZone(zone)) {
        return weather->getWeatherType();
    }
    return WeatherType::CLEAR_WEATHER;
}

void Board::applyWeather(WeatherType type, const std::vector<CombatZone>& zones, int value) {
    weatherEffects.erase(
        std::remove_if(weatherEffects.begin(), weatherEffects.end(),
            [&zones](const auto& w) {
                auto* wc = dynamic_cast<WeatherCard*>(w.get());
                return wc && std::any_of(zones.begin(), zones.end(),
                    [wc](auto z) { return wc->affectsZone(z); });
            }),
        weatherEffects.end()
    );

    auto weatherCard = std::make_unique<WeatherCard>(
        "Weather", type, zones, value
    );
    weatherEffects.push_back(std::move(weatherCard));

    for (auto zone : zones) {
        applyWeatherEffectsToZone(type, zone, value);
    }
}

void Board::applyWeatherEffectsToZone(WeatherType type, CombatZone zone, int value) {
    for (auto& pb : playerBoards) {
        if (!pb.zones.count(zone)) continue;
        
        auto& cards = pb.zones[zone];
        switch(type) {
            case WeatherType::BITING_FROST:
            case WeatherType::IMPENETRABLE_FOG:
            case WeatherType::TORRENTIAL_RAIN:
                for (auto& card : cards) {
                    if (auto unit = dynamic_cast<UnitCard*>(card.get())) {
                        if (!unit->isHeroCard()) {
                            unit->setPower(1);
                            std::cout << "Set " << unit->getName() 
                                      << " power to 1 in " 
                                      << CardUtils::zoneToString(zone) << ".\n";
                        }
                    }
                }
                break;
                
            case WeatherType::CLEAR_WEATHER:
                break;
        }
    }
}

void Board::clearWeather() {
    std::set<CombatZone> affectedZones;

    for (const auto& weather : weatherEffects) {
        if (auto* wc = dynamic_cast<const WeatherCard*>(weather.get())) {
            for (auto zone : wc->getAffectedZones()) {
                if (zone == CombatZone::ANY) {
                    affectedZones.insert(CombatZone::CLOSE);
                    affectedZones.insert(CombatZone::RANGED);
                    affectedZones.insert(CombatZone::SIEGE);
                } else {
                    affectedZones.insert(zone);
                }
            }
        }
    }

    weatherEffects.clear();
    std::cout << "☀️ All weather effects cleared.\n";

    for (auto& pb : playerBoards) {
        for (auto zone : affectedZones) {
            if (!pb.zones.count(zone)) continue;
            
            for (auto& card : pb.zones[zone]) {
                if (auto unit = dynamic_cast<UnitCard*>(card.get())) {
                    if (!unit->isHeroCard()) {
                        unit->setPower(unit->getBasePower());
                        std::cout << "🔄 Restored " << unit->getName() 
                                  << " to base power (" 
                                  << unit->getPower() << ").\n";
                    }
                }
            }
        }
    }
}

bool Board::hasWeather(WeatherType type) const {
    return std::any_of(weatherEffects.begin(), weatherEffects.end(),
        [type](const auto& w) {
            if (auto wc = dynamic_cast<const WeatherCard*>(w.get())) {
                return wc->getWeatherType() == type;
            }
            return false;
        });
}

bool Board::hasWeather(CombatZone zone) const {
    return getActiveWeatherForZone(zone) != nullptr;
}

void Board::cleanupDestroyedUnits(int playerId, CombatZone zone) {
    if (playerId < 0 || playerId >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    auto& zoneUnits = playerBoards[playerId].zones[zone];
    auto& graveyard = playerBoards[playerId].graveyard;

    auto newEnd = std::remove_if(zoneUnits.begin(), zoneUnits.end(),
        [&graveyard](std::unique_ptr<Card>& unit) {
            if (unit->getPower() <= 0) {
                graveyard.push_back(std::move(unit));
                return true;
            }
            return false;
        });

    zoneUnits.erase(newEnd, zoneUnits.end());
}

int Board::getPlayerPower(int playerIndex, CombatZone zone) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    int total = 0;
    const auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (const auto& card : zones.at(zone)) {
            total += card->getPower();
        }
    }
    return total;
}

std::vector<Card*> Board::getPlayerUnits(int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    std::vector<Card*> units;
    for (const auto& [zone, cards] : playerBoards[playerIndex].zones) {
        for (const auto& card : cards) {
            units.push_back(card.get());
        }
    }
    return units;
}

std::vector<Card*> Board::getPlayerUnits(int playerIndex, CombatZone zone) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    std::vector<Card*> units;
    const auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (const auto& card : zones.at(zone)) {
            units.push_back(card.get());
        }
    }
    return units;
}

void Board::boostRow(int playerIndex, CombatZone zone, int effectValue, const Card* exclude) {
    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (auto& card : zones[zone]) {
            if (auto unit = dynamic_cast<UnitCard*>(card.get())) {
                if (!unit->isHeroCard() && card.get() != exclude) {
                    unit->setPower(unit->getPower() + effectValue);
                }
            }
        }
    }
}

void Board::doubleRowPower(int playerIndex, CombatZone zone) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (auto& card : zones[zone]) {
            card->setPower(card->getPower() * 2);
        }
    }
}

void Board::damageRow(int playerIndex, CombatZone zone, int damage) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        auto& cards = zones[zone];
        for (size_t i = 0; i < cards.size(); ) {
            cards[i]->takeDamage(damage);
            if (cards[i]->getPower() <= 0) {
                playerBoards[playerIndex].graveyard.push_back(std::move(cards[i]));
                cards.erase(cards.begin() + i);
            } else {
                ++i;
            }
        }
    }
}

std::string Board::destroyWeakestUnit(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index.");
    }

    Card* weakest = nullptr;
    CombatZone weakestZone;
    size_t weakestIndex = 0;
    int minPower = std::numeric_limits<int>::max();

    for (auto& [zone, cards] : playerBoards[playerIndex].zones) {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (auto unit = dynamic_cast<UnitCard*>(cards[i].get())) {
                if (unit->isHeroCard()) {
                    continue;
                }
            }
            if (cards[i]->getPower() < minPower) {
                minPower = cards[i]->getPower();
                weakest = cards[i].get();
                weakestZone = zone;
                weakestIndex = i;
            }
        }
    }

    if (weakest) {
        std::string destroyedName = weakest->getName();
        
        playerBoards[playerIndex].graveyard.push_back(
            std::move(playerBoards[playerIndex].zones[weakestZone][weakestIndex]));
        playerBoards[playerIndex].zones[weakestZone].erase(
            playerBoards[playerIndex].zones[weakestZone].begin() + weakestIndex);
        
        return destroyedName;
    }
    return "";
}

ScorchResult Board::destroyStrongestEnemyUnit(int attackingPlayerId, Card* activatingCard) {
    ScorchResult result;
    int enemyPlayerId = 1 - attackingPlayerId;
    
    Card* strongestEnemy = nullptr;
    CombatZone strongestZone;
    size_t strongestIndex = 0;
    int maxPower = 0;

    for (auto& [zone, cards] : playerBoards[enemyPlayerId].zones) {
        for (size_t i = 0; i < cards.size(); ++i) {
            bool isSelf = (activatingCard && cards[i].get() == activatingCard);
            if (!isSelf && cards[i]->getPower() > maxPower) {
                maxPower = cards[i]->getPower();
                strongestEnemy = cards[i].get();
                strongestZone = zone;
                strongestIndex = i;
            }
        }
    }

    if (strongestEnemy && maxPower > 0) {
        result.destroyedName = strongestEnemy->getName();
        result.power = strongestEnemy->getPower();
        result.zone = strongestZone;
        result.wasHero = (strongestEnemy->getType() == CardType::HERO);

        playerBoards[enemyPlayerId].graveyard.push_back(
            std::move(playerBoards[enemyPlayerId].zones[strongestZone][strongestIndex]));
        playerBoards[enemyPlayerId].zones[strongestZone].erase(
            playerBoards[enemyPlayerId].zones[strongestZone].begin() + strongestIndex);
    }

    return result;
}

void Board::clearBoard() {
    for (auto& pb : playerBoards) {
        for (auto& [zone, cards] : pb.zones) {
            for (auto& card : cards) {
                pb.graveyard.push_back(std::move(card));
            }
            cards.clear();
        }
    }
    weatherEffects.clear();
}

bool Board::hasUnitsInZone(int playerId, CombatZone zone) const {
    if (playerId < 0 || playerId >= playerBoards.size()) return false;
    const auto& zones = playerBoards[playerId].zones;
    auto it = zones.find(zone);
    return it != zones.end() && !it->second.empty();
}
