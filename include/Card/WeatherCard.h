#ifndef GWENT_WEATHERCARD_H
#define GWENT_WEATHERCARD_H

#include <SFML/Graphics.hpp>
#include "../Card/Card.h"
#include "../Utils/enums.h"
#include <vector>

class WeatherCard : public Card {
private:
    WeatherType weatherType;
    std::vector<CombatZone> affectedZones;
    int effectValue;

    public:
    WeatherCard(const std::string& name, WeatherType type, 
        const std::vector<CombatZone>& affectedZones, int effectValue);
    bool affectsZone(CombatZone zone) const;
    const std::vector<CombatZone>& getAffectedZones() const;
    void play(Player& owner, Player& opponent, Board& board) override;
    void applyEffect(Player& owner, Player& opponent, Board& board) override;
    WeatherType getWeatherType() const override;
    int getEffectValue() const;
    sf::FloatRect getGlobalBounds() const;

    static std::string weatherEffectDescription(WeatherType type);
};

#endif