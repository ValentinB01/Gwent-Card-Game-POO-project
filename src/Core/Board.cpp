#include "../include/Core/Board.h"
#include "../include/Card/UnitCard.h"
#include "../include/Card/WeatherCard.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

void Board::addCard(int playerIndex, std::unique_ptr<Card> card) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    if (card->getType() == CardType::WEATHER) {
        auto weather = dynamic_cast<WeatherCard*>(card.get());
        if (weather) {
            addWeather(std::move(card));
            return;
        }
    }
    playerBoards[playerIndex].zones[card->getZone()].push_back(std::move(card));
}

void Board::addWeather(std::unique_ptr<Card> weatherCard) {
    if (auto weather = dynamic_cast<WeatherCard*>(weatherCard.get())) {
        // Remove existing weather effects for this zone
        weatherEffects.erase(
            remove_if(weatherEffects.begin(), weatherEffects.end(),
                [&weather](const auto& w) { 
                    if (auto wc = dynamic_cast<const WeatherCard*>(w.get())) {
                        return wc->getZone() == weather->getZone(); 
                    }
                    return false;
                }),
            weatherEffects.end());
            
        weatherEffects.push_back(std::move(weatherCard));
        applyWeather(weather->getWeatherType(), weather->getZone(), weather->getEffectValue());
    }
}

WeatherType Board::getWeatherType(CombatZone zone) const {
    for (const auto& weather : weatherEffects) {
        // Safely downcast to WeatherCard
        const WeatherCard* weatherCard = dynamic_cast<const WeatherCard*>(weather.get());
        if (weatherCard && 
            (weatherCard->getZone() == zone || weatherCard->getZone() == CombatZone::ANY)) {
            return weatherCard->getWeatherType();
        }
    }
    return WeatherType::CLEAR_WEATHER;
}

std::vector<std::unique_ptr<Card>>& Board::getPlayerGraveyard(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }
    return playerBoards[playerIndex].graveyard;
}

const std::vector<std::unique_ptr<Card>>& Board::getPlayerGraveyard(int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }
    return playerBoards[playerIndex].graveyard;
}

void Board::applyWeather(WeatherType type, CombatZone zone, int value) {
    switch(type) {
        case WeatherType::BITING_FROST:
        case WeatherType::IMPENETRABLE_FOG:
        case WeatherType::TORRENTIAL_RAIN:
            for (auto& pb : playerBoards) {
                for (auto& card : pb.zones[zone]) {
                    if (auto unit = dynamic_cast<UnitCard*>(card.get())) {
                        if (!unit->isHeroCard()) {
                            unit->setPower(1);
                        }
                    }
                }
            }
            break;
            
        case WeatherType::SKELIGE_STORM:
            for (auto& pb : playerBoards) {
                for (auto& [z, cards] : pb.zones) {
                    if (!cards.empty()) {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<> distr(0, cards.size()-1);
                        int randomIndex = distr(gen);
                        cards[randomIndex]->takeDamage(value);
                        if (cards[randomIndex]->getPower() <= 0) {
                            pb.graveyard.push_back(std::move(cards[randomIndex]));
                            cards.erase(cards.begin() + randomIndex);
                        }
                    }
                }
            }
            break;
            
        case WeatherType::DRAGON_DREAM:
            for (auto& pb : playerBoards) {
                for (auto& [z, cards] : pb.zones) {
                    if (!cards.empty()) {
                        auto strongest = std::max_element(
                            cards.begin(),
                            cards.end(),
                            [](const auto& a, const auto& b) {
                                return a->getPower() < b->getPower();
                            }
                        );
                        (*strongest)->takeDamage(value);
                        if ((*strongest)->getPower() <= 0) {
                            pb.graveyard.push_back(std::move(*strongest));
                            cards.erase(strongest);
                        }
                    }
                }
            }
            break;
            
        case WeatherType::CLEAR_WEATHER:
            clearWeather();
            break;
    }
}

void Board::clearWeather() {
    weatherEffects.clear();
    // Restore original power to affected units
    for (auto& pb : playerBoards) {
        for (auto& [zone, cards] : pb.zones) {
            for (auto& card : cards) {
                if (auto unit = dynamic_cast<UnitCard*>(card.get())) {
                    if (!unit->isHeroCard()) {
                        // Note: This assumes units remember their base power
                        // You may need to modify Card class to track base power
                    }
                }
            }
        }
    }
}

bool Board::hasWeather(WeatherType type) const {
    return std::any_of(weatherEffects.begin(), weatherEffects.end(),
        [type](const auto& w) { return w->getWeatherType() == type; });
}

bool Board::hasWeather(CombatZone zone) const {
    return std::any_of(weatherEffects.begin(), weatherEffects.end(),
        [zone](const auto& w) { 
            return w->getZone() == zone || w->getZone() == CombatZone::ANY; 
        });
}

int Board::getPlayerPower(int playerIndex, CombatZone zone) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    int total = 0;
    const auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (const auto& card : zones.at(zone)) {
            total += card->getPower();
        }
    }
    return total;
}

std::vector<Card*> Board::getPlayerUnits(int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    std::vector<Card*> units;
    for (const auto& [zone, cards] : playerBoards[playerIndex].zones) {
        for (const auto& card : cards) {
            units.push_back(card.get());
        }
    }
    return units;
}

std::vector<Card*> Board::getPlayerUnits(int playerIndex, CombatZone zone) const {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    std::vector<Card*> units;
    const auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (const auto& card : zones.at(zone)) {
            units.push_back(card.get());
        }
    }
    return units;
}

void Board::boostRow(int playerIndex, CombatZone zone, int boostValue) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (auto& card : zones[zone]) {
            card->setPower(card->getPower() + boostValue);
        }
    }
}

void Board::doubleRowPower(int playerIndex, CombatZone zone) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        for (auto& card : zones[zone]) {
            card->setPower(card->getPower() * 2);
        }
    }
}

void Board::damageRow(int playerIndex, CombatZone zone, int damage) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    auto& zones = playerBoards[playerIndex].zones;
    if (zones.count(zone)) {
        auto& cards = zones[zone];
        for (size_t i = 0; i < cards.size(); ) {
            cards[i]->takeDamage(damage);
            if (cards[i]->getPower() <= 0) {
                playerBoards[playerIndex].graveyard.push_back(std::move(cards[i]));
                cards.erase(cards.begin() + i);
            } else {
                ++i;
            }
        }
    }
}

void Board::destroyWeakestUnit(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    Card* weakest = nullptr;
    CombatZone weakestZone;
    size_t weakestIndex = 0;
    int minPower = std::numeric_limits<int>::max();

    for (auto& [zone, cards] : playerBoards[playerIndex].zones) {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (cards[i]->getPower() < minPower) {
                minPower = cards[i]->getPower();
                weakest = cards[i].get();
                weakestZone = zone;
                weakestIndex = i;
            }
        }
    }

    if (weakest) {
        playerBoards[playerIndex].graveyard.push_back(
            std::move(playerBoards[playerIndex].zones[weakestZone][weakestIndex]));
        playerBoards[playerIndex].zones[weakestZone].erase(
            playerBoards[playerIndex].zones[weakestZone].begin() + weakestIndex);
    }
}

void Board::destroyStrongestUnits(int playerIndex, int threshold) {
    if (playerIndex < 0 || playerIndex >= playerBoards.size()) {
        throw std::out_of_range("Invalid player index");
    }

    std::vector<std::pair<CombatZone, size_t>> toDestroy;
    int maxPower = 0;

    // First find the maximum power
    for (auto& [zone, cards] : playerBoards[playerIndex].zones) {
        for (const auto& card : cards) {
            if (card->getPower() > maxPower) {
                maxPower = card->getPower();
            }
        }
    }

    // Skip if no units meet the threshold
    if (maxPower < threshold) return;

    // Collect all strongest units
    for (auto& [zone, cards] : playerBoards[playerIndex].zones) {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (cards[i]->getPower() == maxPower) {
                toDestroy.emplace_back(zone, i);
            }
        }
    }

    // Destroy from back to front to preserve indices
    std::sort(toDestroy.rbegin(), toDestroy.rend());
    for (auto& [zone, index] : toDestroy) {
        playerBoards[playerIndex].graveyard.push_back(
            std::move(playerBoards[playerIndex].zones[zone][index]));
        playerBoards[playerIndex].zones[zone].erase(
            playerBoards[playerIndex].zones[zone].begin() + index);
    }
}

void Board::clearBoard() {
    for (auto& pb : playerBoards) {
        for (auto& [zone, cards] : pb.zones) {
            for (auto& card : cards) {
                pb.graveyard.push_back(std::move(card));
            }
            cards.clear();
        }
    }
    weatherEffects.clear();
}