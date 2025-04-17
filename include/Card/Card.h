#ifndef GWENT_CARD_H
#define GWENT_CARD_H

#include "../Utils/enums.h"
#include <string>
#include <memory>

class Player;
class Board;

class Card {
protected:
    std::string name;
    int power;
    CardType type;
    CombatZone zone;
    Faction faction;
    std::string description;

public:
    Card(const std::string& name, int power, CardType type, CombatZone zone, 
         Faction faction, const std::string& description);
    virtual ~Card() = default;

    virtual void play(Player& owner, Player& opponent, Board& board) = 0;
    virtual void applyEffect(Player& owner, Player& opponent, Board& board) = 0;

    const std::string& getName() const;
    int getPower() const;
    void setPower(int newPower);
    CardType getType() const;
    CombatZone getZone() const;
    Faction getFaction() const;
    const std::string& getDescription() const;
    virtual void takeDamage(int amount);

    virtual WeatherType getWeatherType() const { 
        return WeatherType::CLEAR_WEATHER; // Default implementation
    }
};

#endif