#include "../include/Core/Deck.h"
#include "../include/Card/UnitCard.h"
#include "../include/Card/HeroCard.h"
#include "../include/Card/AbilityCard.h"
#include "../include/Card/WeatherCard.h"
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../json.hpp"

using json = nlohmann::json;

DeployEffect Deck::stringToDeployEffect(const std::string& str) {
    static const std::map<std::string, DeployEffect> effects = {
        {"DAMAGE_RANDOM_ENEMY", DeployEffect::DAMAGE_RANDOM_ENEMY},
        {"BOOST_ADJACENT", DeployEffect::BOOST_ADJACENT},
        {"DRAW_CARD", DeployEffect::DRAW_CARD},
        {"DESTROY_WEAKEST", DeployEffect::DESTROY_WEAKEST},
        {"CLEAR_WEATHER", DeployEffect::CLEAR_WEATHER},
        {"SPY", DeployEffect::SPY},
        {"MEDIC", DeployEffect::MEDIC},
        {"MORALE_BOOST", DeployEffect::MORALE_BOOST}
    };
    auto it = effects.find(str);
    return it != effects.end() ? it->second : DeployEffect::NONE;
}

HeroAbility Deck::stringToHeroAbility(const std::string& str) {
    static const std::map<std::string, HeroAbility> abilities = {
        {"COMMANDERS_HORN", HeroAbility::COMMANDERS_HORN},
        {"SCORCH", HeroAbility::SCORCH},
        {"DECOY", HeroAbility::DECOY},
        {"ALCHEMY", HeroAbility::ALCHEMY},
        {"REVENGE", HeroAbility::REVENGE}
    };
    auto it = abilities.find(str);
    return it != abilities.end() ? it->second : HeroAbility::COMMANDERS_HORN;
}

AbilityEffect Deck::stringToAbilityEffect(const std::string& str) {
    static const std::map<std::string, AbilityEffect> effects = {
        {"DAMAGE_ROW", AbilityEffect::DAMAGE_ROW},
        {"CLEAR_SKIES", AbilityEffect::CLEAR_SKIES},
        {"FOGLET_SPAWN", AbilityEffect::FOGLET_SPAWN},
        {"COMMANDO_TRAINING", AbilityEffect::COMMANDO_TRAINING},
        {"VENOM_EXTRACT", AbilityEffect::VENOM_EXTRACT}
    };
    auto it = effects.find(str);
    return it != effects.end() ? it->second : AbilityEffect::DAMAGE_ROW;
}

WeatherType Deck::stringToWeatherType(const std::string& str) {
    static const std::map<std::string, WeatherType> types = {
        {"BITING_FROST", WeatherType::BITING_FROST},
        {"IMPENETRABLE_FOG", WeatherType::IMPENETRABLE_FOG},
        {"TORRENTIAL_RAIN", WeatherType::TORRENTIAL_RAIN},
        {"CLEAR_WEATHER", WeatherType::CLEAR_WEATHER},
        {"SKELIGE_STORM", WeatherType::SKELIGE_STORM},
        {"DRAGON_DREAM", WeatherType::DRAGON_DREAM}
    };
    auto it = types.find(str);
    return it != types.end() ? it->second : WeatherType::CLEAR_WEATHER;
}

CombatZone Deck::stringToCombatZone(const std::string& str) {
    static const std::map<std::string, CombatZone> zones = {
        {"CLOSE", CombatZone::CLOSE},
        {"RANGED", CombatZone::RANGED},
        {"SIEGE", CombatZone::SIEGE},
        {"ANY", CombatZone::ANY}
    };
    auto it = zones.find(str);
    return it != zones.end() ? it->second : CombatZone::ANY;
}

Faction Deck::stringToFaction(const std::string& str) {
    static const std::map<std::string, Faction> factions = {
        {"NORTH", Faction::NORTH},
        {"SCOIATAEL", Faction::SCOIATAEL},
        {"NILFGARD", Faction::NILFGARD},
        {"MONSTERS", Faction::MONSTERS},
        {"NEUTRAL", Faction::NEUTRAL}
    };
    auto it = factions.find(str);
    return it != factions.end() ? it->second : Faction::NEUTRAL;
}

void Deck::loadFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }

    if (!j.contains("cards") || !j["cards"].is_array()) {
        throw std::runtime_error("Invalid JSON format: missing 'cards' array");
    }

    for (const auto& cardData : j["cards"]) {
        try {
            std::string cardType = cardData["type"];
            std::string name = cardData["name"];
            Faction faction = stringToFaction(cardData["faction"]);
            CombatZone zone = stringToCombatZone(cardData["zone"]);

            if (cardType == "UNIT") {
                int power = cardData["power"];
                bool isHero = cardData.value("isHero", false);
                bool isSpy = cardData.value("isSpy", false);
                DeployEffect effect = stringToDeployEffect(cardData.value("effect", "NONE"));
                int effectValue = cardData.value("effectValue", 0);

                cards.push_back(std::make_unique<UnitCard>(
                    name, power, zone, faction, isHero, effect, effectValue, isSpy
                ));
            }
            else if (cardType == "HERO") {
                int power = cardData["power"];
                HeroAbility ability = stringToHeroAbility(cardData["ability"]);
                int abilityValue = cardData.value("abilityValue", 0);

                cards.push_back(std::make_unique<HeroCard>(
                    name, power, zone, faction, ability, abilityValue
                ));
            }
            else if (cardType == "ABILITY") {
                AbilityEffect effect = stringToAbilityEffect(cardData["effect"]);
                int effectValue = cardData.value("effectValue", 0);
                bool targetsEnemy = cardData.value("targetsEnemy", true);

                cards.push_back(std::make_unique<AbilityCard>(
                    name, zone, faction, effect, effectValue, targetsEnemy
                ));
            }
            else if (cardType == "WEATHER") {
                WeatherType weatherType = stringToWeatherType(cardData["weatherType"]);
                int effectValue = cardData.value("effectValue", 0);
                
                std::vector<CombatZone> affectedZones;
                
                if (cardData.contains("affectedZones")) {
                    for (const auto& zoneStr : cardData["affectedZones"]) {
                        affectedZones.push_back(stringToCombatZone(zoneStr));
                    }
                } else {
                    affectedZones = {getDefaultZoneForWeather(weatherType)};
                }
                
                cards.push_back(std::make_unique<WeatherCard>(
                    name, 
                    weatherType, 
                    affectedZones,
                    effectValue
                ));
            }
        } catch (const json::exception& e) {
            std::cerr << "Error loading card: " << e.what() << std::endl;
            continue;
        }
    }

    std::cout << "Loaded " << cards.size() << " cards from " << filename << std::endl;
}
CombatZone Deck::getDefaultZoneForWeather(WeatherType type) const {
    switch(type) {
        case WeatherType::BITING_FROST:
            return CombatZone::CLOSE;
        case WeatherType::IMPENETRABLE_FOG:
            return CombatZone::RANGED;
        case WeatherType::TORRENTIAL_RAIN:
            return CombatZone::SIEGE;
        case WeatherType::SKELIGE_STORM:
            return CombatZone::ANY;
        case WeatherType::DRAGON_DREAM:
            return CombatZone::ANY;   
        case WeatherType::CLEAR_WEATHER:
            return CombatZone::ANY;     
        default:
            throw std::invalid_argument("Unknown weather type");
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    std::cout << "Deck shuffled (" << cards.size() << " cards)" << std::endl;
}

std::unique_ptr<Card> Deck::drawCard() {
    if (cards.empty()) {
        if (!graveyard.empty()) {
            reshuffleGraveyard();
        } else {
            return nullptr;
        }
    }

    std::unique_ptr<Card> card = std::move(cards.back());
    cards.pop_back();
    return card;
}

void Deck::addCard(std::unique_ptr<Card> card) {
    cards.push_back(std::move(card));
}

void Deck::addToGraveyard(std::unique_ptr<Card> card) {
    graveyard.push_back(std::move(card));
}

size_t Deck::size() const {
    return cards.size();
}

size_t Deck::graveyardSize() const {
    return graveyard.size();
}

void Deck::reshuffleGraveyard() {
    for (auto& card : graveyard) {
        cards.push_back(std::move(card));
    }
    graveyard.clear();
    shuffle();
    std::cout << "Reshuffled graveyard into deck (" << cards.size() << " cards)" << std::endl;
}