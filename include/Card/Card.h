#ifndef GWENT_CARD_H
#define GWENT_CARD_H

#include <SFML/Graphics.hpp>
#include "../Utils/enums.h"
#include <string>
#include <memory>

class Player;
class Board;

enum class CardKind {
    Unit,
    Hero,
    SiegeMachine,
    WeatherEffect,
    Ability
  };
class Card {
protected:
    std::string name;
    int power;
    CardType type;
    CombatZone zone;
    Faction faction;
    std::string description;

public:
    sf::Vector2f position;
    sf::Vector2f size;

    bool positionSet = false;
    
    void setPosition(float x, float y) {
        position = sf::Vector2f(x, y);
        positionSet = true;
    }
    
    Card(const std::string& name, int power, CardType type, CombatZone zone, 
         Faction faction, const std::string& description);
    virtual ~Card() = default;
    virtual void play(Player& owner, Player& opponent, Board& board) = 0;
    virtual void applyEffect(Player& owner, Player& opponent, Board& board) = 0;

    virtual sf::FloatRect getGlobalBounds() const = 0;
    const std::string& getName() const;
    int getPower() const;
    void setPower(int newPower);
    CardType getType() const;
    CombatZone getZone() const;
    Faction getFaction() const;
    const std::string& getDescription() const;
    virtual void takeDamage(int amount);

    virtual WeatherType getWeatherType() const { 
        return WeatherType::NONE;
    }
    
};

#endif