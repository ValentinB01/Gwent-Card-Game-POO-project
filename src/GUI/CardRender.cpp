#include "GUI/CardRender.h"
#include <iostream>

CardRenderer::CardRenderer() {
    if (!font.loadFromFile("../assets/fonts/OldLondon.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    
    factionColors = {
        {Faction::NORTH, sf::Color(70, 130, 180)},    // Steel blue
        {Faction::NILFGARD, sf::Color(47, 79, 79)},     // Dark slate gray
        {Faction::SCOIATAEL, sf::Color(34, 139, 34)},   // Forest green
        {Faction::MONSTERS, sf::Color(139, 0, 0)},      // Dark red
        {Faction::NEUTRAL, sf::Color(169, 169, 169)}    // Dark gray
    };
    
    typeColors = {
        {CardType::UNIT, sf::Color(255, 215, 0)},       // Gold
        {CardType::HERO, sf::Color(218, 165, 32)},      // Golden rod
        {CardType::ABILITY, sf::Color(75, 0, 130)},     // Indigo
        {CardType::WEATHER, sf::Color(135, 206, 235)}   // Sky steel
    };
    
    weatherColors = {
        {WeatherType::BITING_FROST, sf::Color(173, 216, 230)},  // Light blue
        {WeatherType::IMPENETRABLE_FOG, sf::Color(211, 211, 211)}, // Light gray
        {WeatherType::TORRENTIAL_RAIN, sf::Color(65, 105, 225)}, // Royal blue
        {WeatherType::CLEAR_WEATHER, sf::Color(255, 255, 255)},  // White
        {WeatherType::SKELIGE_STORM, sf::Color(138, 43, 226)},   // Blue violet
        {WeatherType::DRAGON_DREAM, sf::Color(220, 20, 60)}      // Crimson
    };
}

bool CardRenderer::loadResources() {
    if (!cardBackTexture.loadFromFile("../assets/images/card_back.png")) {
        std::cerr << "Failed to load card back texture\n";
        return false;
    }
    cardBackTexture.setSmooth(true);
    cardBackTexture.setRepeated(false);
    
    if (!cardBaseTexture.loadFromFile("../assets/images/card_base.png")) {
        std::cerr << "Failed to load card base texture\n";
        return false;
    }
    cardBaseTexture.setSmooth(true);
    cardBaseTexture.setRepeated(false);

    return true;
}

void CardRenderer::renderCard(sf::RenderTarget& target, const Card& card, float x, float y, bool highlight) {
    sf::RectangleShape cardBase(getCardSize());
    setupCardBase(cardBase, card);
    cardBase.setPosition(x, y);
    
    if (highlight) {
        cardBase.setOutlineColor(sf::Color::Yellow);
        cardBase.setOutlineThickness(3.f);
    }
    
    target.draw(cardBase);
    
    sf::Text nameText;
    setupCardText(nameText, card.getName(), 14, x + 10, y + 10);
    target.draw(nameText);
    
    if (card.getType() != CardType::WEATHER && card.getType() != CardType::ABILITY) {
        renderCardPower(target, card, x + getCardSize().x - 30, y + getCardSize().y - 30);
    }
    
    renderCardInfo(target, card, x, y);
}

void CardRenderer::renderCardBack(sf::RenderTarget& target, float x, float y) {
    sf::Sprite cardBack(cardBackTexture);
    cardBack.setPosition(x, y);
    cardBack.setScale(
        getCardSize().x / cardBack.getLocalBounds().width,
        getCardSize().y / cardBack.getLocalBounds().height
    );
    target.draw(cardBack);
}

void CardRenderer::renderCardInfo(sf::RenderTarget& target, const Card& card, float x, float y) {
    sf::Text typeText;
    std::string typeStr;
    
    switch(card.getType()) {
        case CardType::UNIT:
            typeStr = "Unit";
            if (dynamic_cast<const UnitCard*>(&card)->getIsSpy()) {
                typeStr += " (Spy)";
            }
            break;
        case CardType::HERO: typeStr = "Hero"; break;
        case CardType::ABILITY: typeStr = "Ability"; break;
        case CardType::WEATHER: typeStr = "Weather"; break;
    }
    
    setupCardText(typeText, typeStr, 12, x + 10, y + 30);
    target.draw(typeText);
    
    sf::Text zoneText;
    std::string zoneStr;
    switch(card.getZone()) {
        case CombatZone::CLOSE: zoneStr = "Close"; break;
        case CombatZone::RANGED: zoneStr = "Ranged"; break;
        case CombatZone::SIEGE: zoneStr = "Siege"; break;
        case CombatZone::ANY: zoneStr = "Any"; break;
    }
    
    setupCardText(zoneText, zoneStr, 12, x + 10, y + 50);
    target.draw(zoneText);
    
    if (card.getType() == CardType::WEATHER) {
        const WeatherCard* weather = dynamic_cast<const WeatherCard*>(&card);
        sf::RectangleShape weatherIcon(sf::Vector2f(20, 20));
        weatherIcon.setPosition(x + getCardSize().x - 30, y + 10);
        weatherIcon.setFillColor(weatherColors.at(weather->getWeatherType()));
        target.draw(weatherIcon);
    }
}

void CardRenderer::setupCardBase(sf::RectangleShape& cardBase, const Card& card) const {
    cardBase.setTexture(&cardBaseTexture);
    cardBase.setFillColor(factionColors.at(card.getFaction()));
    cardBase.setOutlineColor(sf::Color::Black);
    cardBase.setOutlineThickness(1.f);
}

void CardRenderer::setupCardText(sf::Text& text, const std::string& str, unsigned size, float x, float y) const {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

void CardRenderer::renderCardPower(sf::RenderTarget& target, const Card& card, float x, float y) const {
    sf::CircleShape powerCircle(15);
    powerCircle.setPosition(x, y);
    powerCircle.setFillColor(sf::Color::Black);
    powerCircle.setOutlineColor(sf::Color::White);
    powerCircle.setOutlineThickness(2.f);
    target.draw(powerCircle);
    
    sf::Text powerText;
    setupCardText(powerText, std::to_string(card.getPower()), 16, x + 5, y + 5);
    powerText.setStyle(sf::Text::Bold);
    target.draw(powerText);
}