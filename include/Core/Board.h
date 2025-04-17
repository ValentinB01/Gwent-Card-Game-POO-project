#ifndef GWENT_BOARD_H
#define GWENT_BOARD_H

#include "../Card/Card.h"
#include "../Card/WeatherCard.h"
#include <vector>
#include <memory>
#include <map>
#include <array>

class Board {
private:
    struct PlayerBoard {
        std::map<CombatZone, std::vector<std::unique_ptr<Card>>> zones;
        std::vector<std::unique_ptr<Card>> graveyard;
    };

    std::array<PlayerBoard, 2> playerBoards;
    std::vector<std::unique_ptr<Card>> weatherEffects;

public:
    // Card management
    void addCard(int playerIndex, std::unique_ptr<Card> card);
    
    // Weather effects
    WeatherType getWeatherType(CombatZone zone) const;
    void addWeather(std::unique_ptr<Card> weatherCard);
    void applyWeather(WeatherType type, CombatZone zone, int value);
    void clearWeather();
    bool hasWeather(WeatherType type) const;
    bool hasWeather(CombatZone zone) const;
    
    // Graveyard access
    std::vector<std::unique_ptr<Card>>& getPlayerGraveyard(int playerIndex);
    const std::vector<std::unique_ptr<Card>>& getPlayerGraveyard(int playerIndex) const;
    
    // Board state queries
    int getPlayerPower(int playerIndex, CombatZone zone) const;
    std::vector<Card*> getPlayerUnits(int playerIndex) const;
    std::vector<Card*> getPlayerUnits(int playerIndex, CombatZone zone) const;
    
    // Board modifications
    void boostRow(int playerIndex, CombatZone zone, int boostValue);
    void doubleRowPower(int playerIndex, CombatZone zone);
    void damageRow(int playerIndex, CombatZone zone, int damage);
    void destroyWeakestUnit(int playerIndex);
    void destroyStrongestUnits(int playerIndex, int threshold);
    void clearBoard();
};

#endif