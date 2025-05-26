#pragma once

#include "../Card/Card.h"
#include "../Card/WeatherCard.h"
#include <vector>
#include <memory>
#include <map>
#include <array>

struct ScorchResult {
    std::string destroyedName;
    int power;
    CombatZone zone;
    bool wasHero;
};

class Board {
private:
    struct PlayerBoard {
        std::map<CombatZone, std::vector<std::unique_ptr<Card>>> zones;
        std::vector<std::unique_ptr<Card>> graveyard;
    };
    std::array<PlayerBoard, 2> playerBoards;
    std::vector<std::unique_ptr<Card>> weatherEffects;

public:
    void addCard(int playerIndex, std::unique_ptr<Card> card);
    void cleanupDestroyedUnits(int playerId, CombatZone zone);
    ScorchResult destroyStrongestEnemyUnit(int attackingPlayerId, Card* activatingCard = nullptr);   
    WeatherType getWeatherType(CombatZone zone) const;
    const WeatherCard* getActiveWeatherForZone(CombatZone zone) const;
    void addWeather(std::unique_ptr<Card> weatherCard);
    void applyWeather(WeatherType type, const std::vector<CombatZone>& zones, int value);
    void clearWeather();
    bool hasWeather(WeatherType type) const;
    bool hasWeather(CombatZone zone) const;
    bool shouldApplyWeather(CombatZone weatherZone, CombatZone targetZone) const;
    void applyWeatherEffectsToZone(WeatherType type, CombatZone zone, int value);

    std::vector<std::unique_ptr<Card>>& getPlayerGraveyard(int playerIndex);
    const std::vector<std::unique_ptr<Card>>& getPlayerGraveyard(int playerIndex) const;
    
    int getPlayerPower(int playerIndex, CombatZone zone) const;
    std::vector<Card*> getPlayerUnits(int playerIndex) const;
    std::vector<Card*> getPlayerUnits(int playerIndex, CombatZone zone) const;
    
    void boostRow(int playerIndex, CombatZone zone, int boostValue, const Card* exclude = nullptr);
    void doubleRowPower(int playerIndex, CombatZone zone);
    void damageRow(int playerIndex, CombatZone zone, int damage);
    std::string destroyWeakestUnit(int playerIndex);
    void clearBoard();
    bool hasUnitsInZone(int playerId, CombatZone zone) const;

    std::vector<std::unique_ptr<Card>>& getPlayerZone(int playerIndex, CombatZone zone);
    const std::vector<std::unique_ptr<Card>>& getPlayerZone(int playerIndex, CombatZone zone) const;
};
