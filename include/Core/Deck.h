#pragma once

#include "../Card/Card.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

class Deck {
private:
    std::vector<std::unique_ptr<Card>> cards;
    std::vector<std::unique_ptr<Card>> graveyard;
    
    DeployEffect stringToDeployEffect(const std::string& str);
    HeroAbility stringToHeroAbility(const std::string& str);
    AbilityEffect stringToAbilityEffect(const std::string& str);
    WeatherType stringToWeatherType(const std::string& str);
    CombatZone stringToCombatZone(const std::string& str);
    Faction stringToFaction(const std::string& str);

public:
    Deck() = default;
    
    void loadFromJson(const std::string& filename);
    void shuffle();
    std::unique_ptr<Card> drawCard();
    void addCard(std::unique_ptr<Card> card);
    void addToGraveyard(std::unique_ptr<Card> card);
    size_t size() const;
    size_t graveyardSize() const;
    void reshuffleGraveyard();
    CombatZone getDefaultZoneForWeather(WeatherType type) const;
};
