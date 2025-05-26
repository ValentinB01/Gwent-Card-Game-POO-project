#include "../include/Card/Card.h"
#include <iostream>

Card::Card(const std::string& name, int power, CardType type, CombatZone zone, 
           Faction faction, const std::string& description)
    : name(name), power(power), type(type), zone(zone), 
      faction(faction), description(description) {}

const std::string& Card::getName() const { return name; }
int Card::getPower() const { return power; }
void Card::setPower(int newPower) { power = newPower; }
CardType Card::getType() const { return type; }
CombatZone Card::getZone() const { return zone; }
Faction Card::getFaction() const { return faction; }
const std::string& Card::getDescription() const { return description; }

void Card::takeDamage(int amount) {
    power -= amount;
    if (power <= 0) {
        std::cout << name << " has been destroyed!" << std::endl;
    }
}
WeatherType Card::getWeatherType() const { 
        return WeatherType::NONE;
}
void Card::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    positionSet = true;
}
