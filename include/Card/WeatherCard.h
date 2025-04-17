#ifndef GWENT_WEATHERCARD_H
#define GWENT_WEATHERCARD_H

#include "../Card/Card.h"
#include "../Utils/enums.h"

class WeatherCard : public Card {
private:
    WeatherType weatherType;
    int effectValue;

public:
    WeatherCard(const std::string& name, CombatZone zone, 
                WeatherType type, int effectValue = 0);
    
    // Overridden virtual functions
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    
    // Getters
    WeatherType getWeatherType() const override;
    int getEffectValue() const;
    
    // Static helper
    static std::string weatherEffectDescription(WeatherType type);
};

#endif