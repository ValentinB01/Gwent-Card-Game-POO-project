#include "../include/Card/WeatherCard.h"
#include "../include/Core/Board.h"
#include "../include/Core/Player.h"
#include "../include/Utils/CardUtils.h"
#include <algorithm>
#include <iostream>

WeatherCard::WeatherCard(const std::string& name, WeatherType type, 
    const std::vector<CombatZone>& affectedZones, int effectValue)
: Card(name, 0, CardType::WEATHER, CombatZone::ANY, Faction::NEUTRAL, 
    "Weather effect card applies to "+ CardUtils::zoneToString(zone) + " battle zone."),
  weatherType(type), affectedZones(affectedZones), effectValue(effectValue) {}

void WeatherCard::play(Player& owner, Player& opponent, Board& board) {
    std::cout << CardUtils::weatherTypeToSymbol(weatherType) << " Weather played: " << name 
              << " | Affected Zones: ";
    for (auto zone : affectedZones) {
        std::cout << CardUtils::zoneToString(zone) << " ";
    }
    std::cout << std::endl;
    
    applyEffect(owner, opponent, board);
}

void WeatherCard::applyEffect(Player& owner, Player& opponent, Board& board) {
    if (weatherType == WeatherType::CLEAR_WEATHER) {
        board.clearWeather();
        std::cout << "Weather effects cleared!" << std::endl;
        return;
    }

    board.applyWeather(weatherType, affectedZones, effectValue);
    std::cout << CardUtils::weatherEffectDescription(weatherType) << std::endl;
}

bool WeatherCard::affectsZone(CombatZone zone) const {
    return std::find(affectedZones.begin(), affectedZones.end(), zone) != affectedZones.end() ||
           std::find(affectedZones.begin(), affectedZones.end(), CombatZone::ANY) != affectedZones.end();
}

WeatherType WeatherCard::getWeatherType() const { 
    return weatherType; 
}

int WeatherCard::getEffectValue() const { 
    return effectValue; 
}

const std::vector<CombatZone>& WeatherCard::getAffectedZones() const {
    return affectedZones;
}
sf::FloatRect WeatherCard::getGlobalBounds() const {
    return sf::FloatRect(position.x, position.y, size.x, size.y);
}
