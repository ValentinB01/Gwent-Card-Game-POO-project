#include "../include/Card/WeatherCard.h"
#include "../include/Core/Board.h"
#include "../include/Core/Player.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>

WeatherCard::WeatherCard(const std::string& name, CombatZone zone, 
                         WeatherType type, int effectValue)
    : Card(name, 0, CardType::WEATHER, zone, Faction::NEUTRAL, 
           "Weather effect card"),
      weatherType(type), effectValue(effectValue) {}

void WeatherCard::play(Player& owner, Player& opponent, Board& board) {
    std::cout << CardUtils::weatherTypeToSymbol(weatherType) << " Weather played: " << name 
              << " | Zone: " << CardUtils::zoneToString(zone) << std::endl;
    applyEffect(owner, opponent, board);
}

void WeatherCard::applyEffect(Player& owner, Player& opponent, Board& board) {
    if (weatherType == WeatherType::CLEAR_WEATHER) {
        board.clearWeather();
        std::cout << "Weather effects cleared!" << std::endl;
        return;
    }

    board.applyWeather(weatherType, zone, effectValue);
    std::cout << CardUtils::weatherEffectDescription(weatherType) << std::endl;
}

WeatherType WeatherCard::getWeatherType() const { return weatherType; }
int WeatherCard::getEffectValue() const { return effectValue; }